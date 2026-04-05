class PNH_DatabaseManager
{
    private static ref PNH_DatabaseManager m_Instance;
    private ref map<string, ref PNH_PlayerData> m_PlayerCache; 
    private string m_RootPath = "$profile:PNH/Players/";

    void PNH_DatabaseManager()
    {
        m_PlayerCache = new map<string, ref PNH_PlayerData>;
        
        // A engine exige que a pasta mãe seja criada primeiro
        if (!FileExist("$profile:PNH"))
        {
            MakeDirectory("$profile:PNH");
            Print("[PNH_DB] Pasta raiz PNH criada.");
        }
        
        // Depois criamos a subpasta
        if (!FileExist("$profile:PNH/Players"))
        {
            MakeDirectory("$profile:PNH/Players");
            Print("[PNH_DB] Subpasta Players criada.");
        }
    }

    static PNH_DatabaseManager GetInstance()
    {
        if (!m_Instance)
        {
            m_Instance = new PNH_DatabaseManager();
        }
        return m_Instance;
    }

    // Carrega do HD para a Memória
    PNH_PlayerData LoadPlayer(string steamID)
    {
        if (m_PlayerCache.Contains(steamID))
        {
            return m_PlayerCache.Get(steamID);
        }

        string path = m_RootPath + steamID + ".json";
        ref PNH_PlayerData data = new PNH_PlayerData(steamID);

        if (FileExist(path))
        {
            JsonFileLoader<PNH_PlayerData>.JsonLoadFile(path, data);
            Print("[PNH_DB] Dados carregados para: " + steamID);
        }
        else
        {
            SavePlayerToDisk(data); 
            Print("[PNH_DB] Novo perfil criado para: " + steamID);
        }

        m_PlayerCache.Insert(steamID, data);
        return data;
    }

    // Salva da Memória para o HD
    void SavePlayerToDisk(PNH_PlayerData data)
    {
        if (!data) return;

        string path = m_RootPath + data.SteamID + ".json";
        JsonFileLoader<PNH_PlayerData>.JsonSaveFile(path, data);
        Print("[PNH_DB] Dados salvos no disco para: " + data.SteamID);
    }

    // Remove da memória quando o jogador sai
    void UnloadPlayer(string steamID)
    {
        if (m_PlayerCache.Contains(steamID))
        {
            SavePlayerToDisk(m_PlayerCache.Get(steamID));
            m_PlayerCache.Remove(steamID);
            Print("[PNH_DB] Dados removidos do cache para: " + steamID);
        }
    }

    // Função rápida para acessar dados de um jogador online
    PNH_PlayerData GetCachedPlayer(string steamID)
    {
        return m_PlayerCache.Get(steamID);
    }
}