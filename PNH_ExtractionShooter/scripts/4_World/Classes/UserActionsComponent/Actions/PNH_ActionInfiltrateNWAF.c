class PNH_ActionInfiltrateNWAF: ActionSingleUseBase
{
    void PNH_ActionInfiltrateNWAF()
    {
        // Usa uma animação rápida de mão
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
        // Trava: O jogador precisa estar num raio de 150m da Prisão para o cartão funcionar
        if (vector.Distance(player.GetPosition(), "2669 10 1310".ToVector()) > 150.0) return false;
        
        return true;
    }

    override void OnExecuteServer(ActionData action_data)
    {
        PNH_RaidInstance raid = PNH_RaidPoolManager.GetInstance().GetRaid("Raid_NWAF_01");
        if (raid)
        {
            // O nosso truque de teste: Zera a raid para você entrar limpo
            raid.RaidTimer = 0;
            raid.State = EPNHRaidState.ACTIVE;
            Print("[PNH_Action] Tempo da Raid NWAF resetado via Ação do Cartão Físico.");
            
            PNH_InfiltrationManager.Infiltrate(action_data.m_Player, raid);
        }
    }
}

// Injeta a nossa nova ação no sistema central do jogo
modded class ActionConstructor
{
    override void RegisterActions(TTypenameArray actions)
    {
        super.RegisterActions(actions);
        actions.Insert(PNH_ActionInfiltrateNWAF);
    }
}