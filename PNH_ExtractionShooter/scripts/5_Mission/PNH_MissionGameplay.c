modded class MissionGameplay
{
    ref PNH_MapSelectionMenu m_PNHMenu;

    override void OnKeyPress(int key)
    {
        super.OnKeyPress(key);
        
        if (key == KeyCode.KC_H) 
        {
            if (GetGame().GetUIManager().GetMenu() == NULL)
            {
                if (!m_PNHMenu)
                {
                    m_PNHMenu = new PNH_MapSelectionMenu;
                }
                // O Init() é chamado internamente aqui pelo motor
                GetGame().GetUIManager().ShowScriptedMenu(m_PNHMenu, null);
            }
            else if (m_PNHMenu && GetGame().GetUIManager().GetMenu() == m_PNHMenu)
            {
                GetGame().GetUIManager().HideScriptedMenu(m_PNHMenu);
            }
        }
    }
}