modded class MissionGameplay
{
    ref PNH_MapSelectionMenu m_PNHMenu;

    override void OnKeyPress(int key)
    {
        super.OnKeyPress(key);
        
        if (key == KeyCode.KC_H) 
        {
            // Trava 1: Garante que não sobrepõe o inventário ou o chat (Cursor não visível)
            if (GetGame().GetUIManager().GetMenu() == NULL && !GetGame().GetUIManager().IsCursorVisible())
            {
                PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
                if (player)
                {
                    // Trava 2: Geofence - Verifica a distância do HUB (Prisão) no Cliente
                    vector hubCenter = "2669.597900 10.569989 1310.054810";
                    float hubRadius = 600.0;

                    if (vector.Distance(player.GetPosition(), hubCenter) <= hubRadius)
                    {
                        if (!m_PNHMenu)
                        {
                            m_PNHMenu = new PNH_MapSelectionMenu;
                        }
                        // O Init() é chamado internamente aqui pelo motor
                        GetGame().GetUIManager().ShowScriptedMenu(m_PNHMenu, null);
                    }
                    else
                    {
                        // Mensagem local informando o jogador do porquê o menu não abrir
                        GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", "Acesso Negado: O Terminal só está disponível no HUB (Prisão).", ""));
                    }
                }
            }
            // MANTIDO: Lógica original que fecha o menu se a tecla for pressionada novamente
            else if (m_PNHMenu && GetGame().GetUIManager().GetMenu() == m_PNHMenu)
            {
                GetGame().GetUIManager().HideScriptedMenu(m_PNHMenu);
            }
        }
    }
}