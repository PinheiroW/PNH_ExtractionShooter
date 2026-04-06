class PNH_InfiltrationManager
{
    static bool CanInfiltrate(PlayerBase player, PNH_RaidInstance raid)
    {
        string playerName = player.GetIdentity().GetName();
        Print("[PNH_Infiltration] Iniciando checagem de requisitos para o jogador: " + playerName);

        if (raid.RaidTimer > (600.0 * 0.7)) 
        {
            Print("[PNH_Infiltration] FALHA: Raid em estágio final. Tempo atual: " + raid.RaidTimer.ToString());
            return false;
        }

        if (raid.RaidID.Contains("NWAF"))
        {
            EntityAI card = null;

            Print("[PNH_Infiltration] Verificando as mãos do jogador " + playerName + "...");
            EntityAI itemInHands = player.GetHumanInventory().GetEntityInHands();
            
            if (itemInHands && itemInHands.GetType() == "PNH_AccessCard_NWAF")
            {
                card = itemInHands;
                Print("[PNH_Infiltration] Cartão PNH_AccessCard_NWAF encontrado nas MÃOS.");
            }
            else
            {
                Print("[PNH_Infiltration] Cartão não está nas mãos. Varrendo inventário/mochila...");
                array<EntityAI> items = new array<EntityAI>;
                player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);

                foreach (EntityAI item : items)
                {
                    if (item.GetType() == "PNH_AccessCard_NWAF")
                    {
                        card = item;
                        Print("[PNH_Infiltration] Cartão PNH_AccessCard_NWAF encontrado no INVENTÁRIO.");
                        break; 
                    }
                }
            }
            
            if (!card)
            {
                Print("[PNH_Infiltration] FALHA: O jogador " + playerName + " tentou entrar sem o cartão necessário.");
                return false;
            }
            
            card.Delete(); 
            Print("[PNH_Infiltration] SUCESSO: Cartão consumido do inventário de " + playerName + ".");
        }

        return true;
    }

    static void Infiltrate(PlayerBase player, PNH_RaidInstance raid)
    {
        if (!CanInfiltrate(player, raid)) return;

        string steamID = player.GetIdentity().GetPlainId();
        string playerName = player.GetIdentity().GetName();
        Print("[PNH_Infiltration] Autorização concedida. Preparando teleporte para " + playerName);

        PNH_PlayerData data = PNH_DatabaseManager.GetInstance().GetCachedPlayer(steamID);
        
        data.ActiveRaidID = raid.RaidID;
        raid.PlayerSteamIDs.Insert(steamID);
        Print("[PNH_Infiltration] Jogador vinculado à instância: " + raid.RaidID);
        
        vector spawnPos = GetRandomBorderSpawn(raid.RaidID);
        player.SetPosition(spawnPos);
        Print("[PNH_Infiltration] Teleporte executado. Destino: " + spawnPos.ToString());
        
        PNH_SpawnProtection.Activate(player);
        Print("[PNH_Infiltration] God Mode temporário ativado para proteção de spawn.");
        
        PNH_DatabaseManager.GetInstance().SavePlayerToDisk(data);
        Print("[PNH_Infiltration] Dados de infiltração salvos no banco de dados para " + steamID);
    }

    static vector GetRandomBorderSpawn(string raidID)
    {
        if (raidID == "Raid_NWAF_01")
        {
            array<vector> nwafSpawns = new array<vector>;
            
            nwafSpawns.Insert("5080.008301 351.539001 9020.171875".ToVector());
            nwafSpawns.Insert("5178.707520 313.590393 10294.606445".ToVector());
            nwafSpawns.Insert("4469.422363 303.069580 11322.601563".ToVector());
            nwafSpawns.Insert("4043.863037 292.666901 12072.010742".ToVector());
            nwafSpawns.Insert("3568.587646 336.709595 11464.992188".ToVector());
            nwafSpawns.Insert("3785.334473 344.951355 10555.202148".ToVector());
            nwafSpawns.Insert("4311.410156 339.065643 9843.549805".ToVector());

            int randomIndex = Math.RandomInt(0, nwafSpawns.Count());
            Print("[PNH_Infiltration] Realizando sorteio de spawn. Índice escolhido: " + randomIndex.ToString() + " de " + nwafSpawns.Count().ToString());
            return nwafSpawns.Get(randomIndex);
        }
        
        return "10000 0 10000".ToVector(); 
    }
}