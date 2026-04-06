// PNH_MapConfig.c
// Responsável por carregar os dados dos mapas. Inclui proteção contra kick no cliente.

class PNH_MapData
{
    string id;
    string name;
    string imagePath;
    string levelOfLoot;
    string status;
    string timeRemaining; 
    string playersOnline; 
    string timeToOpen;    
    string playersInQueue; 
    string description;

    void PNH_MapData(string mapId = "")
    {
        id = mapId;
    }
}

class PNH_MapConfig
{
    ref array<ref PNH_MapData> maps;

    void PNH_MapConfig()
    {
        maps = new array<ref PNH_MapData>();
    }

    static ref PNH_MapConfig LoadConfig(string configPath)
    {
        ref PNH_MapConfig config = new PNH_MapConfig();
        
        // Trava para evitar que o Cliente tente ler arquivos locais e seja expulso do servidor
        if (GetGame().IsServer() || !GetGame().IsMultiplayer())
        {
            if (FileExist(configPath))
            {
                JsonFileLoader<ref PNH_MapConfig>.JsonLoadFile(configPath, config);
                Print("[PNH_Config] Dados carregados do arquivo JSON.");
            }
        }
        
        // Dados padrões caso o arquivo não seja encontrado ou estejamos no Cliente
        if (config.maps.Count() == 0)
        {
            config.PopulateDefaults();
        }

        return config;
    }

    void PopulateDefaults()
    {
        ref PNH_MapData m1 = new PNH_MapData("NWAF");
        m1.name = "North West Airfield";
        m1.imagePath = "set:dayz_gui_pnh image:pnh_nwaf_thumbnail";
        m1.levelOfLoot = "Alto";
        m1.status = "Aberto";
        m1.timeRemaining = "00:45:00";
        m1.playersOnline = "42/60";
        m1.description = "Zona de alto risco e alta recompensa.";
        maps.Insert(m1);

        ref PNH_MapData m2 = new PNH_MapData("Chernogorsk");
        m2.name = "Chernogorsk";
        m2.imagePath = "set:dayz_gui_pnh image:pnh_chernogorsk_thumbnail";
        m2.status = "Fechado";
        m2.timeToOpen = "00:12:45";
        m2.playersInQueue = "15";
        m2.description = "Ambiente urbano denso e perigoso.";
        maps.Insert(m2);
        
        ref PNH_MapData m3 = new PNH_MapData("Tisy");
        m3.name = "Tisy Military Base";
        m3.imagePath = "set:dayz_gui_pnh image:pnh_tisy_thumbnail";
        m3.levelOfLoot = "Extremo";
        m3.status = "Aberto";
        m3.timeRemaining = "01:10:30";
        m3.playersOnline = "55/60";
        m3.description = "O ponto mais perigoso de Chernarus.";
        maps.Insert(m3);
    }
}