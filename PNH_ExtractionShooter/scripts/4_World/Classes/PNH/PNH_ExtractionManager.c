class PNH_ExtractionManager
{
    private static const float EXTRACTION_TIME = 10.0; 
    private static const float EXTRACTION_RADIUS = 15.0; 

    private static ref array<vector> m_ExtractionsNWAF;      
    private static ref array<vector> m_PaidExtractionsNWAF;  
    private static ref map<string, float> m_PlayerTimers; 

    static void Init()
    {
        // 1. Zonas Gratuitas (Exigem 10s de espera)
        m_ExtractionsNWAF = new array<vector>;
        m_ExtractionsNWAF.Insert("4391.616699 339.625427 10232.226563".ToVector());
        m_ExtractionsNWAF.Insert("4457.392090 339.195251 10725.411133".ToVector());
        
        // 2. Zonas Pagas (Ação Física Imediata)
        m_PaidExtractionsNWAF = new array<vector>;
        m_PaidExtractionsNWAF.Insert("4023.442139 336.627869 10357.059570".ToVector());
        m_PaidExtractionsNWAF.Insert("5050.468750 343.939667 9567.325195".ToVector());

        m_PlayerTimers = new map<string, float>;
        Print("[PNH_Extraction] Sistema de Extração Inicializado. (2 Gratuitas | 2 Pagas/Físicas)");
    }

    // Radar com Cálculo 2D: Ignora a altura (Y) para evitar bugs de desnível no terreno
    static bool IsPaidExtractionZone(vector playerPos)
    {
        if (!m_PaidExtractionsNWAF) return false;
        
        foreach (vector paidExtPos : m_PaidExtractionsNWAF)
        {
            float dX = playerPos[0] - paidExtPos[0];
            float dZ = playerPos[2] - paidExtPos[2];
            float dist2D = Math.Sqrt((dX * dX) + (dZ * dZ));
            
            if (dist2D <= EXTRACTION_RADIUS) return true;
        }
        return false;
    }

    static void Update(float timeslice)
    {
        if (!m_ExtractionsNWAF || !GetGame()) return; 

        array<Man> players = new array<Man>;
        GetGame().GetPlayers(players);

        foreach (Man man : players)
        {
            PlayerBase player = PlayerBase.Cast(man);
            if (!player || !player.IsAlive() || !player.GetIdentity()) continue;

            string steamID = player.GetIdentity().GetPlainId();
            string playerName = player.GetIdentity().GetName(); 
            
            PNH_PlayerData data = PNH_DatabaseManager.GetInstance().GetCachedPlayer(steamID);
            if (!data || data.ActiveRaidID == "") continue;

            bool inFreeZone = false;
            
            if (data.ActiveRaidID == "Raid_NWAF_01")
            {
                foreach (vector extPos : m_ExtractionsNWAF)
                {
                    // Cálculo 2D também para as zonas gratuitas
                    float dX = player.GetPosition()[0] - extPos[0];
                    float dZ = player.GetPosition()[2] - extPos[2];
                    float dist2D = Math.Sqrt((dX * dX) + (dZ * dZ));

                    if (dist2D <= EXTRACTION_RADIUS)
                    {
                        inFreeZone = true;
                        break;
                    }
                }
            }

            // Lógica Exclusiva para as Zonas Gratuitas (Timer de 10s)
            if (inFreeZone)
            {
                if (!m_PlayerTimers.Contains(steamID) || m_PlayerTimers.Get(steamID) < 0) 
                {
                    m_PlayerTimers.Set(steamID, 0);
                    Print("[PNH_Extraction] ALERTA: " + playerName + " iniciou extração GRATUITA.");
                    GetGame().RPCSingleParam(player, ERPCs.RPC_USER_ACTION_MESSAGE, new Param1<string>("ZONA DE EXTRAÇÃO ALCANÇADA\nSobreviva por 10 segundos!"), true, player.GetIdentity());
                }

                float currentTime = m_PlayerTimers.Get(steamID) + timeslice;
                currentTime = Math.Min(currentTime, EXTRACTION_TIME + 1.0); 
                m_PlayerTimers.Set(steamID, currentTime);

                if (currentTime >= EXTRACTION_TIME)
                {
                    ExecuteExtraction(player, data, playerName);
                    m_PlayerTimers.Remove(steamID); 
                }
            }
            else
            {
                if (m_PlayerTimers.Contains(steamID))
                {
                    if (m_PlayerTimers.Get(steamID) >= 0) 
                    {
                        Print("[PNH_Extraction] CANCELADO: " + playerName + " fugiu da zona gratuita.");
                        GetGame().RPCSingleParam(player, ERPCs.RPC_USER_ACTION_MESSAGE, new Param1<string>("EXTRAÇÃO CANCELADA\nVocê saiu da zona de segurança!"), true, player.GetIdentity());
                    }
                    m_PlayerTimers.Remove(steamID);
                }
            }
        }
    }

    static void ExecuteExtraction(PlayerBase player, PNH_PlayerData data, string playerName)
    {
        vector hubPos = "2669.597900 10.569989 1310.054810".ToVector();
        player.SetPosition(hubPos); 
        Print("[PNH_Extraction] Teleporte executado. " + playerName + " retornado para a base (HUB).");
        
        data.ActiveRaidID = "";
        PNH_DatabaseManager.GetInstance().SavePlayerToDisk(data);
        
        GetGame().RPCSingleParam(player, ERPCs.RPC_USER_ACTION_MESSAGE, new Param1<string>("EXTRAÇÃO CONCLUÍDA\nBem-vindo de volta à Prisão!"), true, player.GetIdentity());
    }
}