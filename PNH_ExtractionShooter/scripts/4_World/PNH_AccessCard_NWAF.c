class PNH_AccessCard_NWAF : Paper
{
    override void SetActions()
    {
        super.SetActions();
        
        // Adiciona a opção de usar o cartão fisicamente
        AddAction(PNH_ActionInfiltrateNWAF);
    }
}