modded class MissionServer
{
    private float m_PNH_CleanupTimer = 0;

    override void OnInit()
    {
        super.OnInit();
        PNH_DatabaseManager.GetInstance(); 
        Print("[PNH_Core] Sistema de Persistência Inicializado com Sucesso.");
        
        PNH_RaidPoolManager.GetInstance();
        Print("[PNH_Core] Sistema de Raid Pool Inicializado.");

        // Acorda o monitorador de pontos de extração
        PNH_ExtractionManager.Init();
    }

    override void OnUpdate(float timeslice)
    {
        super.OnUpdate(timeslice);
        
        m_PNH_CleanupTimer += timeslice;
        if (m_PNH_CleanupTimer >= 300) 
        {
            PNH_HubCleanup.RunCleanup();
            m_PNH_CleanupTimer = 0; 
        }

        // Faz o relógio das Raids e o radar das Extrações rodarem
        PNH_RaidPoolManager.GetInstance().Update(timeslice);
        PNH_ExtractionManager.Update(timeslice);
    }

    // --- SISTEMA TEMPORÁRIO DE COMANDOS DE CHAT ---
    override void OnEvent(EventType eventTypeId, Param params)
    {
        super.OnEvent(eventTypeId, params);

        if (eventTypeId == ChatMessageEventTypeID)
        {
            ChatMessageEventParams chatParams;
            CastTo(chatParams, params);

            if (chatParams)
            {
                string text = chatParams.param3;
                text.ToLower(); 

                if (text.Contains("!entrar nwaf"))
                {
                    string senderName = chatParams.param2; 
                    
                    array<Man> players = new array<Man>;
                    GetGame().GetPlayers(players);
                    
                    foreach (Man man : players)
                    {
                        PlayerBase player = PlayerBase.Cast(man);
                        if (player && player.GetIdentity() && player.GetIdentity().GetName() == senderName)
                        {
                            PNH_RaidInstance raid = PNH_RaidPoolManager.GetInstance().GetRaid("Raid_NWAF_01");
                            if (raid)
                            {
                                raid.RaidTimer = 0;
                                raid.State = EPNHRaidState.ACTIVE;
                                Print("[PNH_Admin] Tempo da Raid NWAF resetado via comando de chat.");

                                PNH_InfiltrationManager.Infiltrate(player, raid);
                            }
                            break;
                        }
                    }
                }
            }
        }
    }

    override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
    {
        super.InvokeOnConnect(player, identity);

        if (identity)
        {
            string steamID = identity.GetPlainId();
            PNH_PlayerData data = PNH_DatabaseManager.GetInstance().LoadPlayer(steamID);

            if (data.PendingWipe)
            {
                Print("[PNH_Core] Punição aplicada: Jogador " + steamID + " sofreu Wipe Offline.");
                GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ExecuteOfflineWipe, 2000, false, player);
                data.PendingWipe = false; 
                PNH_DatabaseManager.GetInstance().SavePlayerToDisk(data);
            }

            Print("[PNH_Core] Bem-vindo de volta, " + identity.GetName() + ". Saldo Virtual: " + data.BankBalance);
        }
    }

    override void InvokeOnDisconnect(PlayerBase player)
    {
        super.InvokeOnDisconnect(player);

        if (player && player.GetIdentity())
        {
            string steamID = player.GetIdentity().GetPlainId();
            PNH_DatabaseManager.GetInstance().UnloadPlayer(steamID);
        }
    }

    void ExecuteOfflineWipe(PlayerBase player)
    {
        // TRAVA 1: Verifica se o jogador existe e está vivo
        if (!player || !player.IsAlive()) return;
        
        // TRAVA 2: Verifica se o inventário está bloqueado pelo motor do jogo
        if (player.GetInventory() && player.GetInventory().IsInventoryLocked()) 
        {
            Print("[PNH_Wipe] ATENÇÃO: Inventário de " + player.GetIdentity().GetName() + " bloqueado. Wipe abortado para evitar crash.");
            return;
        }

        // Se passou pelas travas, pode limpar com segurança
        Print("[PNH_Wipe] Executando Wipe Offline Seguro para " + player.GetIdentity().GetName());
        player.RemoveAllItems();
        
        // Teleporta de volta para a Prisão (HUB) após limpar o inventário
        vector hubPos = "2669.597900 10.569989 1310.054810".ToVector();
        player.SetPosition(hubPos);
    }
}