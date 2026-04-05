modded class MissionGameplay
{
    ref PNH_MapSelectionMenu m_PNHMenu;

    override void OnKeyPress(int key)
    {
        super.OnKeyPress(key);
        
        // Verifica se a tecla pressionada foi a tecla 'H'
        if (key == KeyCode.KC_H) 
        {
            // Se nenhum outro menu (como o inventário ou menu do ESC) estiver aberto
            if (GetGame().GetUIManager().GetMenu() == NULL)
            {
                // Se a nossa tela ainda não existe na memória, cria ela
                if (!m_PNHMenu)
                {
                    m_PNHMenu = new PNH_MapSelectionMenu;
                    m_PNHMenu.Init();
                }
                // Mostra a tela
                GetGame().GetUIManager().ShowScriptedMenu(m_PNHMenu, null);
            }
            // Se a nossa tela já estiver aberta, aperta 'H' de novo para fechar
            else if (m_PNHMenu && GetGame().GetUIManager().GetMenu() == m_PNHMenu)
            {
                GetGame().GetUIManager().HideScriptedMenu(m_PNHMenu);
            }
        }
    }
}