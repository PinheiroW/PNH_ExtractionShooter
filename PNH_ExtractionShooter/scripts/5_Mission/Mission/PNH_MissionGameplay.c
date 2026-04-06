modded class MissionGameplay
{
    protected ref PNH_MapSelectionMenu m_PNHMenu;

    override void OnKeyPress(int key)
    {
        super.OnKeyPress(key);

        if (key == KeyCode.KC_H && GetGame().GetUIManager().GetMenu() == NULL)
        {
            PlayerBase p = PlayerBase.Cast(GetGame().GetPlayer());
            if (p && PNH_SafeZoneManager.IsInHub(p.GetPosition()))
            {
                // Abre o menu usando o método nativo seguro
                m_PNHMenu = PNH_MapSelectionMenu.Cast(GetGame().GetUIManager().EnterScriptedMenu(PNH_MapSelectionMenu, null));
            }
        }
    }
}