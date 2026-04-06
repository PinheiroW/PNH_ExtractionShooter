// PNH_PlayerBase.c
// Gerenciamento de estado do jogador (SafeZone) e Recepção de UI

modded class PlayerBase
{
    // ==========================================
    // PARTE 1: A SUA SAFEZONE (MANTIDA INTACTA)
    // ==========================================
    private float m_PNH_SafeZoneTimer = 0;

    override void OnScheduledTick(float deltaTime)
    {
        super.OnScheduledTick(deltaTime);

        m_PNH_SafeZoneTimer += deltaTime;

        if (m_PNH_SafeZoneTimer >= 1.0)
        {
            if (PNH_SafeZoneManager.IsInHub(this.GetPosition()))
            {
                this.SetAllowDamage(false);
            }
            else
            {
                this.SetAllowDamage(true);
            }
            m_PNH_SafeZoneTimer = 0;
        }
    }

    override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
    {
        if (PNH_SafeZoneManager.IsInHub(this.GetPosition())) return;
        super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
    }

    // ==========================================
    // PARTE 2: A NOVA PONTE COM A INTERFACE (UI)
    // ==========================================
    override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
    {
        super.OnRPC(sender, rpc_type, ctx);

        // Ouve o sinal 54321 vindo do botão "Ingressar" do Menu
        if (GetGame().IsServer() && rpc_type == 54321) 
        {
            // Validação 1: Anti-Spoofing (Segurança)
            if (sender == null || sender.GetId() != GetIdentity().GetId()) return;

            // Validação 2: Geofence (Impede que usem a UI fora da prisão)
            if (!PNH_SafeZoneManager.IsInHub(this.GetPosition())) return;

            Param1<string> raidParam;
            if (!ctx.Read(raidParam)) return;

            string mapID = raidParam.param1; 
            string raidID = "Raid_" + mapID + "_01"; 
            
            PNH_RaidInstance raid = PNH_RaidPoolManager.GetInstance().GetRaid(raidID);
            
            if (raid && (raid.State == EPNHRaidState.ACTIVE || raid.State == EPNHRaidState.WAITING))
            {
                // Validação 3: Pagamento ATM (Cobra 5000 de saldo virtual)
                int custoInfiltracao = 5000; 
                
                if (PNH_ATMManager.GetInstance().CanAffordInfiltration(this, custoInfiltracao))
                {
                    PNH_ATMManager.GetInstance().DeductBalance(this, custoInfiltracao);
                    
                    if (raid.State == EPNHRaidState.WAITING) raid.State = EPNHRaidState.ACTIVE;
                    
                    Print("[PNH_RPC] Pagamento validado via UI. Infiltrando: " + this.GetIdentity().GetName());
                    PNH_InfiltrationManager.Infiltrate(this, raid);
                }
                else
                {
                    // Se não tiver dinheiro, avisa no chat
                    GetGame().RPCSingleParam(this, ERPCs.RPC_USER_ACTION_MESSAGE, new Param1<string>("SALDO INSUFICIENTE\nVocê não possui dinheiro para a taxa."), true, this.GetIdentity());
                }
            }
        }
    }
}