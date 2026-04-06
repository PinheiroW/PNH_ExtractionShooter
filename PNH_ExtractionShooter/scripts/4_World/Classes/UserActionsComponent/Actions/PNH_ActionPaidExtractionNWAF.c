class PNH_ActionPaidExtractionNWAF: ActionSingleUseBase
{
    void PNH_ActionPaidExtractionNWAF()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_LICKBATTERY; 
        m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
    }

    override void CreateConditionComponents()
    {
        m_ConditionItem = new CCINonRuined;
        m_ConditionTarget = new CCTNone;
    }

    override string GetText()
    {
        return "Pagar Extração VIP (1x Paper)";
    }

    override bool HasTarget()
    {
        return false;
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        // Só aparece a opção de usar o Paper se o jogador estiver pisando numa Zona VIP
        if (!PNH_ExtractionManager.IsPaidExtractionZone(player.GetPosition())) return false;
        
        return true;
    }

    override void OnExecuteServer(ActionData action_data)
    {
        PlayerBase player = action_data.m_Player;
        string steamID = player.GetIdentity().GetPlainId();
        string playerName = player.GetIdentity().GetName();
        
        PNH_PlayerData data = PNH_DatabaseManager.GetInstance().GetCachedPlayer(steamID);
        
        // Verifica se o jogador realmente está em uma Raid antes de extraí-lo
        if (data && data.ActiveRaidID != "")
        {
            // Consome o item da mão (o Paper)
            action_data.m_MainItem.Delete();
            Print("[PNH_Extraction] Pagamento debitado. Extração VIP física executada para " + playerName);
            
            // Teleporta de volta para a Prisão
            vector hubPos = "2669.597900 10.569989 1310.054810".ToVector();
            player.SetPosition(hubPos); 
            
            // Limpa o banco de dados
            data.ActiveRaidID = "";
            PNH_DatabaseManager.GetInstance().SavePlayerToDisk(data);
            
            GetGame().RPCSingleParam(player, ERPCs.RPC_USER_ACTION_MESSAGE, new Param1<string>("EXTRAÇÃO VIP CONCLUÍDA\nBem-vindo de volta à Prisão!"), true, player.GetIdentity());
        }
    }
}

// Injeta a nova ação no sistema
modded class ActionConstructor
{
    override void RegisterActions(TTypenameArray actions)
    {
        super.RegisterActions(actions);
        actions.Insert(PNH_ActionPaidExtractionNWAF);
    }
}