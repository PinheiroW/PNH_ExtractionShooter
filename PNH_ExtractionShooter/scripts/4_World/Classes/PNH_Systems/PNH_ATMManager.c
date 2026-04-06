// PNH_ATMManager.c
// Gerenciamento de saldo virtual e validação de infiltração.

class PNH_ATMManager
{
    protected static ref PNH_ATMManager m_Instance;

    // CORREÇÃO CRÍTICA: Sem o 'ref' no tipo de retorno da função estática
    static PNH_ATMManager GetInstance()
    {
        if (!m_Instance)
        {
            m_Instance = new PNH_ATMManager();
        }
        return m_Instance;
    }

    void PNH_ATMManager() {}

    // Verifica se o jogador tem saldo suficiente para a taxa
    bool CanAffordInfiltration(PlayerBase player, int cost)
    {
        if (!player) return false;

        string steamId = player.GetIdentity().GetPlainId();
        int currentBalance = GetPlayerBalance(steamId);

        if (currentBalance >= cost)
        {
            return true;
        }

        return false;
    }

    // Desconta o valor do saldo do jogador (Lógica Anti-Dupe)
    bool DeductBalance(PlayerBase player, int amount)
    {
        if (!player) return false;

        string steamId = player.GetIdentity().GetPlainId();
        int currentBalance = GetPlayerBalance(steamId);

        if (currentBalance < amount)
        {
            Print("[PNH_ATM] ERRO: Tentativa de saldo negativo para o jogador: " + steamId);
            return false;
        }

        int newBalance = currentBalance - amount;
        SavePlayerBalance(steamId, newBalance);
        
        Print("[PNH_ATM] Taxa de $" + amount + " cobrada com sucesso. Novo saldo de " + player.GetIdentity().GetName() + ": $" + newBalance);
        return true;
    }

    // SIMULAÇÃO DE BANCO DE DADOS (Para testes da UI)
    protected int GetPlayerBalance(string steamId)
    {
        // Placeholder: Retorna 150k de saldo fixo para testes de Infiltração passarem
        return 150000; 
    }

    protected void SavePlayerBalance(string steamId, int balance)
    {
        // No futuro, aqui entrará o JsonFileLoader para salvar o novo saldo no disco.
        Print("[PNH_ATM] Saldo virtual atualizado na memória.");
    }
}