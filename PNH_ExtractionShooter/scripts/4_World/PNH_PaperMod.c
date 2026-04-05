modded class Paper
{
    override void SetActions()
    {
        super.SetActions();
        
        // Adiciona a nossa ação de fuga ao papel comum do jogo
        AddAction(PNH_ActionPaidExtractionNWAF);
    }
}