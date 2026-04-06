modded class PlayerBase
{
    // --- LÓGICA DA ZONA SEGURA (Já existia) ---
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

    // --- NOVA LÓGICA: RECEBER COMANDO DA UI (RPC 54321) ---
    override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
    {
        super.OnRPC(sender, rpc_type, ctx);

        // Se o servidor ouvir a frequência 54321 (Botão Ingressar da UI)
        if (GetGame().IsServer() && rpc_type == 54321) 
        {
            Param1<string> raidParam;
            if (!ctx.Read(raidParam)) return;

            string mapID = raidParam.param1; // O ID que vem do JSON (ex: "NWAF")
            
            // Monta o nome técnico da Raid (ex: "Raid_NWAF_01")
            string raidID = "Raid_" + mapID + "_01"; 
            
            PNH_RaidInstance raid = PNH_RaidPoolManager.GetInstance().GetRaid(raidID);
            
            // Verifica se a Raid existe e se o mapa está com status ACTIVE
            if (raid && raid.State == EPNHRaidState.ACTIVE)
            {
                Print("[PNH_RPC] Pedido de infiltração recebido via UI para: " + this.GetIdentity().GetName() + " no mapa " + mapID);
                
                // Dispara o teleporte e consumo do cartão!
                PNH_InfiltrationManager.Infiltrate(this, raid);
            }
            else
            {
                // Se a raid estiver fechada ou limpando (CLEANING), nega a entrada.
                GetGame().RPCSingleParam(this, ERPCs.RPC_USER_ACTION_MESSAGE, new Param1<string>("INFILTRAÇÃO NEGADA\nA zona de operação não está ativa no momento."), true, this.GetIdentity());
            }
        }
    }
}