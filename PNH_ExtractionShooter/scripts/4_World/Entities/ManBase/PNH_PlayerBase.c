// PNH_PlayerBase.c
// Gerenciamento de estado do jogador (SafeZone)

modded class PlayerBase
{
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
}