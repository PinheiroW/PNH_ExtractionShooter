class PNH_ActionInfiltrateNWAF: ActionSingleUseBase
{
    void PNH_ActionInfiltrateNWAF()
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
        return "Utilizar Cartão (Infiltrar NWAF)";
    }

    override bool HasTarget()
    {
        return false;
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        // Trava: O jogador precisa estar na Prisão (SafeZone) para o cartão funcionar
        if (!PNH_SafeZoneManager.IsInHub(player.GetPosition())) return false;
        
        return (item && item.IsKindOf("PNH_AccessCard_NWAF"));
    }

    override void OnExecuteServer(ActionData action_data)
    {
        PlayerBase player = action_data.m_Player;
        if (!player) return;

        PNH_RaidInstance raid = PNH_RaidPoolManager.GetInstance().GetRaid("Raid_NWAF_01");
        
        // MODIFICAÇÃO: Aceita entrar se estiver ACTIVE ou WAITING
        if (raid && (raid.State == EPNHRaidState.ACTIVE || raid.State == EPNHRaidState.WAITING))
        {
            // Se a raid estava apenas esperando, o uso do cartão a torna ATIVA agora
            if (raid.State == EPNHRaidState.WAITING)
            {
                raid.State = EPNHRaidState.ACTIVE;
                raid.RaidTimer = 0; // Reseta o cronômetro para você ter o tempo total da raid
                Print("[PNH_Action] Raid NWAF ativada pelo uso do cartão do jogador: " + player.GetIdentity().GetName());
            }

            // O InfiltrationManager cuidará do teleporte e de DELETAR o cartão
            PNH_InfiltrationManager.Infiltrate(player, raid);
        }
        else
        {
            // Caso a raid esteja em CLEANING ou REBUILDING
            GetGame().RPCSingleParam(player, ERPCs.RPC_USER_ACTION_MESSAGE, new Param1<string>("INFILTRAÇÃO NEGADA\nA zona de operação está em processo de limpeza."), true, player.GetIdentity());
        }
    }
}

modded class ActionConstructor
{
    override void RegisterActions(TTypenameArray actions)
    {
        super.RegisterActions(actions);
        actions.Insert(PNH_ActionInfiltrateNWAF);
    }
}