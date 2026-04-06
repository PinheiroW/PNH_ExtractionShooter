// PNH_MapConfig.c
class PNH_MapData
{
    string id;
    string name;
    string description;
    string status;
    int playersOnline;
    string timeRemaining;
    string timeToOpen;
}

class PNH_MapConfig
{
    ref array<ref PNH_MapData> maps;

    void PNH_MapConfig()
    {
        maps = new array<ref PNH_MapData>;
    }

    static PNH_MapConfig LoadConfig(string path)
    {
        MakeDirectory("$profile:PNH_ExtractionShooter");
        PNH_MapConfig config = new PNH_MapConfig();

        if (FileExist(path))
        {
            JsonFileLoader<PNH_MapConfig>.JsonLoadFile(path, config);
        }
        else
        {
            config.CreateDefaultConfig(path);
        }
        return config;
    }

    void CreateDefaultConfig(string path)
    {
        PNH_MapData nwaf = new PNH_MapData();
        nwaf.id = "NWAF";
        nwaf.name = "NORTHWEST AIRFIELD";
        nwaf.description = "Zona militar de alto risco.";
        nwaf.status = "Aberto";
        maps.Insert(nwaf);

        PNH_MapData tisy = new PNH_MapData();
        tisy.id = "Tisy";
        tisy.name = "BASE MILITAR TISY";
        tisy.description = "Risco extremo. Contaminacao detectada.";
        tisy.status = "Inativo";
        maps.Insert(tisy);

        JsonFileLoader<PNH_MapConfig>.JsonSaveFile(path, this);
    }
}