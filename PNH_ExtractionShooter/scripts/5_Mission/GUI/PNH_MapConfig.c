// PNH_MapConfig.c
// Esta classe é responsável por carregar e armazenar a configuração dos mapas a partir de um arquivo JSON.

class PNH_MapData
{
	string id;
	string name;
	string imagePath;
	string levelOfLoot;
	string status;
	string timeRemaining; // Para mapas abertos
	string playersOnline; // Para mapas abertos
	string timeToOpen;    // Para mapas fechados
	string playersInQueue; // Para mapas fechados
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

	// Método para carregar a configuração do JSON
	static ref PNH_MapConfig LoadConfig(string configPath)
	{
		ref PNH_MapConfig config = new PNH_MapConfig();
		string fileContent;

		if (FileExist(configPath))
		{
			FileHandle file = OpenFile(configPath, FileMode.READ);
			if (file != 0)
			{
				string line_content;
				while (FGets(file, line_content) > 0)
				{
					fileContent += line_content;
				}
				CloseFile(file);

				JsonFileLoader<ref PNH_MapConfig>.JsonLoadData(fileContent, config);
			}
		}
		
		// Exemplo de dados padrão se o arquivo não for encontrado ou estiver vazio
		if (config.maps.Count() == 0)
		{
			Print("PNH_MapConfig: No maps loaded from config. Using default data.");
			ref PNH_MapData defaultMap1 = new PNH_MapData("NWAF");
			defaultMap1.name = "North West Airfield";
			defaultMap1.imagePath = "set:dayz_gui_pnh image:pnh_nwaf_thumbnail";
			defaultMap1.levelOfLoot = "Alto";
			defaultMap1.status = "Aberto";
			defaultMap1.timeRemaining = "00:45:00";
			defaultMap1.playersOnline = "42/60";
			defaultMap1.description = "O Aeródromo do Noroeste é uma área de alto risco e alta recompensa, ideal para operadores experientes. Espere forte resistência e loot de alto nível.";
			config.maps.Insert(defaultMap1);

			ref PNH_MapData defaultMap2 = new PNH_MapData("Chernogorsk");
			defaultMap2.name = "Chernogorsk";
			defaultMap2.imagePath = "set:dayz_gui_pnh image:pnh_chernogorsk_thumbnail";
			defaultMap2.status = "Fechado";
			defaultMap2.timeToOpen = "00:12:45";
			defaultMap2.playersInQueue = "15";
			defaultMap2.description = "A cidade costeira de Chernogorsk oferece um ambiente urbano denso com oportunidades de combate a curta distância. Atualmente fechada, junte-se à fila para a próxima rotação.";
			config.maps.Insert(defaultMap2);

			ref PNH_MapData defaultMap3 = new PNH_MapData("Tisy");
			defaultMap3.name = "Tisy Military Base";
			defaultMap3.imagePath = "set:dayz_gui_pnh image:pnh_tisy_thumbnail";
			defaultMap3.levelOfLoot = "Extremo";
			defaultMap3.status = "Aberto";
			defaultMap3.timeRemaining = "01:10:30";
			defaultMap3.playersOnline = "55/60";
			defaultMap3.description = "A Base Militar de Tisy é o ponto mais perigoso e lucrativo de Chernarus. Somente os mais preparados sobrevivem. Loot de elite garantido.";
			config.maps.Insert(defaultMap3);
		}

		return config;
	}
}