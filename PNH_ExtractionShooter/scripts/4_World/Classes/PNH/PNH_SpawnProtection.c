class PNH_SpawnProtection
{
    static void Activate(PlayerBase player)
    {
        player.SetAllowDamage(false); 
        Print("[PNH_Raid] Proteção de Spawn ativada para " + player.GetIdentity().GetName());
        
        // A engine espera 5000ms (5 segundos) e chama a função Deactivate sozinha
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Deactivate, 5000, false, player);
    }

    static void Deactivate(PlayerBase player)
    {
        if (player)
        {
            player.SetAllowDamage(true); // O jogador volta a ser mortal
            Print("[PNH_Raid] Proteção de Spawn expirada. Boa sorte!");
        }
    }
}