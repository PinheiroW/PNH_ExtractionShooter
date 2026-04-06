class PNH_AccessCard_NWAF : Paper
{
    override void SetActions()
    {
        super.SetActions();
        
        // Adiciona a ação de interagir ao segurar o cartão
        AddAction(PNH_ActionInfiltrateNWAF);
    }
}