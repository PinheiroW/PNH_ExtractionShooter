// Estados possíveis de uma Raid
enum EPNHRaidState
{
    WAITING,    // Pronta em background
    AVAILABLE,  // Aceitando jogadores no painel
    ACTIVE,     // Em andamento (Timer rodando)
    CLEANING,   // Limpeza de corpos e itens
    REBUILDING  // Reset de loot e IA
}

// O Objeto que representa uma partida individual
class PNH_RaidInstance
{
    string RaidID;
    EPNHRaidState State;
    float RaidTimer; 
    ref array<string> PlayerSteamIDs;

    void PNH_RaidInstance(string id)
    {
        RaidID = id;
        State = EPNHRaidState.WAITING;
        RaidTimer = 0; 
        PlayerSteamIDs = new array<string>;
    }
}

// O Cérebro que controla todas as partidas simultâneas
class PNH_RaidPoolManager
{
    private static ref PNH_RaidPoolManager m_Instance;
    private ref array<ref PNH_RaidInstance> m_Raids;
    
    // Deixei 120 segundos (2 minutos) para o seu teste rápido!
    private const float RAID_DURATION = 120.0; 

    void PNH_RaidPoolManager()
    {
        m_Raids = new array<ref PNH_RaidInstance>;
        
        // Cadastramos os mapas/zonas das suas Raids aqui
        m_Raids.Insert(new PNH_RaidInstance("Raid_NWAF_01"));
        m_Raids.Insert(new PNH_RaidInstance("Raid_Industrial_01"));
    }

    static PNH_RaidPoolManager GetInstance()
    {
        if (!m_Instance) m_Instance = new PNH_RaidPoolManager();
        return m_Instance;
    }

    // Função que será chamada a cada segundo pelo servidor
    void Update(float timeslice)
    {
        foreach (PNH_RaidInstance raid : m_Raids)
        {
            // Alterado para rodar no WAITING só para o seu teste rápido engatar na hora
            if (raid.State == EPNHRaidState.ACTIVE || raid.State == EPNHRaidState.WAITING)
            {
                raid.RaidTimer += timeslice; 
                
                // Se o tempo da raid acabou (2 minutos no teste)
                if (raid.RaidTimer >= RAID_DURATION) 
                {
                    StartCleaning(raid);
                }
            }
        }
    }

    void StartCleaning(PNH_RaidInstance raid)
    {
        raid.State = EPNHRaidState.CLEANING;
        Print("[PNH_Raid] Raid " + raid.RaidID + " entrou em CLEANING. Punindo remanescentes...");
        
        // Varre os jogadores que estavam nessa raid e aplica o Wipe (MIA)
        foreach (string steamID : raid.PlayerSteamIDs)
        {
            PNH_PlayerData data = PNH_DatabaseManager.GetInstance().GetCachedPlayer(steamID);
            
            // Se o jogador ainda estiver com o ActiveRaidID preenchido, ele não extraiu!
            if (data && data.ActiveRaidID == raid.RaidID)
            {
                data.PendingWipe = true; // Aplica a punição
                PNH_DatabaseManager.GetInstance().SavePlayerToDisk(data);
            }
        }
    }

    // NOVA FUNÇÃO: Busca a instância correta da raid pelo nome
    PNH_RaidInstance GetRaid(string raidID)
    {
        foreach (PNH_RaidInstance raid : m_Raids)
        {
            if (raid.RaidID == raidID) 
            {
                return raid;
            }
        }
        return null;
    }
}