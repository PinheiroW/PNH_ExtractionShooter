class PNH_ATMManager
{
    // A classe do dinheiro que o seu mod de dinheiro usa
    private static string m_MoneyClassName = "NOME_DO_ITEM_DE_DINHEIRO"; 

    static void DepositPhysicalMoney(PlayerBase player)
    {
        // Trava de segurança: Só funciona se o jogador estiver dentro dos 600m da Prisão
        if (!player || !PNH_SafeZoneManager.IsInHub(player.GetPosition())) 
        {
            return; 
        }

        int totalToDeposit = 0;
        array<EntityAI> items = new array<EntityAI>;
        
        // Varre tudo que está dentro do inventário do jogador (bolsos, mochila, calça)
        player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);

        foreach (EntityAI item : items)
        {
            if (item.GetType() == m_MoneyClassName)
            {
                // Aqui nós pegamos a quantidade. Se for um maço de 50 notas, vale 50.
                int itemQuantity = item.GetQuantity();
                
                // Se o item não tiver quantidade (for uma nota única), assumimos que vale 1
                if (itemQuantity == 0) itemQuantity = 1; 

                totalToDeposit += itemQuantity; 
                item.Delete(); // Apaga a nota física da mochila
            }
        }

        if (totalToDeposit > 0)
        {
            // Atualiza o seu JSON de sessão que criamos na Fase 1
            PNH_PlayerData data = PNH_DatabaseManager.GetInstance().GetCachedPlayer(player.GetIdentity().GetPlainId());
            if (data)
            {
                data.BankBalance += totalToDeposit;
                PNH_DatabaseManager.GetInstance().SavePlayerToDisk(data);
                
                Print("[PNH_Economy] Depósito de R$ " + totalToDeposit + " realizado por " + player.GetIdentity().GetName());
            }
        }
    }
}