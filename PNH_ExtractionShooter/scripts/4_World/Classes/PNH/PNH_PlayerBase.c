modded class PlayerBase
{
    // Criamos um cronômetro individual para cada jogador
    private float m_PNH_SafeZoneTimer = 0;

    override void OnScheduledTick(float deltaTime)
    {
        super.OnScheduledTick(deltaTime);

        // Somamos o tempo que passou
        m_PNH_SafeZoneTimer += deltaTime;

        // Só executamos o cálculo de distância a cada 1 segundo (1.0)
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
            
            // Zeramos o cronômetro para o próximo segundo
            m_PNH_SafeZoneTimer = 0;
        }
    }

    override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
    {
        if (PNH_SafeZoneManager.IsInHub(this.GetPosition()))
        {
            return;
        }

        super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
    }
}