// PNH_MapSelectionMenu.c
// Este script gerencia a interface de seleção de mapas para o mod PNH_ExtractionShooter.

class PNH_MapSelectionMenu extends UIScriptedMenu
{
    protected Widget m_RootWidget;
    protected TextWidget m_OperatorName;
    protected TextWidget m_ATMBalance;
    protected ButtonWidget m_CloseButton;

    protected GridSpacerWidget m_MapGridSpacer; // Corrigido para GridSpacerWidget
    protected ImageWidget m_MapImage;
    protected TextWidget m_MapTitle;
    protected RichTextWidget m_MapStats;
    protected RichTextWidget m_MapDescription;
    protected ButtonWidget m_ActionButton;

    protected ref PNH_MapConfig m_MapConfig;
    protected ref PNH_MapData m_SelectedMap;

    void PNH_MapSelectionMenu()
    {
        // Construtor
    }

    override Widget Init()
    {
        // Carrega o layout da UI com o caminho completo
        m_RootWidget = GetGame().GetWorkspace().CreateWidgets("PNH_ExtractionShooter/GUI/layouts/PNH_MapSelectionMenu.layout");

        // Inicializa os widgets
        m_OperatorName = TextWidget.Cast(m_RootWidget.FindAnyWidget("PNH_OperatorName"));
        m_ATMBalance = TextWidget.Cast(m_RootWidget.FindAnyWidget("PNH_ATMBalance"));
        m_CloseButton = ButtonWidget.Cast(m_RootWidget.FindAnyWidget("PNH_CloseButton"));

        m_MapGridSpacer = GridSpacerWidget.Cast(m_RootWidget.FindAnyWidget("PNH_MapGridSpacer")); // Corrigido para GridSpacerWidget
        m_MapImage = ImageWidget.Cast(m_RootWidget.FindAnyWidget("PNH_MapImage"));
        m_MapTitle = TextWidget.Cast(m_RootWidget.FindAnyWidget("PNH_MapTitle"));
        m_MapStats = RichTextWidget.Cast(m_RootWidget.FindAnyWidget("PNH_MapStats"));
        m_MapDescription = RichTextWidget.Cast(m_RootWidget.FindAnyWidget("PNH_MapDescription"));
        m_ActionButton = ButtonWidget.Cast(m_RootWidget.FindAnyWidget("PNH_ActionButton"));

        // Carrega a configuração dos mapas do diretório de profiles
        m_MapConfig = PNH_MapConfig.LoadConfig("$profile:PNH/PNH_MapConfig.json"); 

        PopulateMapList();

        return m_RootWidget;
    }

    override void OnShow()
    {
        super.OnShow();
        GetGame().GetInput().ChangeGameFocus(1); // Foca a entrada na UI
        GetGame().GetUIManager().ShowCursor(true); // Mostra o cursor

        // Atualiza o nome do operador usando a identidade real do jogador
        if (GetGame().GetPlayer() && GetGame().GetPlayer().GetIdentity())
        {
            m_OperatorName.SetText("OPERADOR: " + GetGame().GetPlayer().GetIdentity().GetName());
        }
        
        m_ATMBalance.SetText("SALDO ATM: $15,200"); // Placeholder (será conectado ao banco no futuro)

        // Seleciona o primeiro mapa por padrão
        if (m_MapConfig && m_MapConfig.maps.Count() > 0)
        {
            UpdateMapDetails(m_MapConfig.maps.Get(0));
        }
    }

    override void OnHide()
    {
        super.OnHide();
        GetGame().GetInput().ChangeGameFocus(-1); // Retorna o foco para o jogo
        GetGame().GetUIManager().ShowCursor(false); // Esconde o cursor
    }

    // Preenche a lista de mapas no GridSpacer
    protected void PopulateMapList()
    {
        if (!m_MapGridSpacer || !m_MapConfig)
            return;

        // Limpa a lista existente (MÉTODO CORRETO PARA WIDGETS)
        Widget child = m_MapGridSpacer.GetChildren();
        while (child)
        {
            Widget next = child.GetSibling();
            child.Unlink(); // Destrói o widget filho
            child = next;
        }

        // Adiciona cada mapa da configuração como um item clicável
        for (int i = 0; i < m_MapConfig.maps.Count(); i++)
        {
            ref PNH_MapData mapData = m_MapConfig.maps.Get(i);

            // Cria um painel para o item do mapa (com o caminho completo)
            Widget mapItem = GetGame().GetWorkspace().CreateWidgets("PNH_ExtractionShooter/GUI/layouts/PNH_MapListItem.layout", m_MapGridSpacer); 
            TextWidget mapName = TextWidget.Cast(mapItem.FindAnyWidget("PNH_MapListItemName"));
            ImageWidget mapThumbnail = ImageWidget.Cast(mapItem.FindAnyWidget("PNH_MapListItemThumbnail"));

            if (mapName)
            {
                mapName.SetText(mapData.name);
            }
            if (mapThumbnail && mapData.imagePath != "")
            {
                mapThumbnail.LoadImageFile(0, mapData.imagePath);
            }

            mapItem.SetHandler(this); // Define este script como handler para cliques
            mapItem.SetUserData(mapData); // Armazena os dados do mapa no widget para fácil acesso
        }
    }

    // Atualiza os detalhes do mapa no painel direito
    protected void UpdateMapDetails(PNH_MapData mapData)
    {
        if (!mapData)
            return;

        m_SelectedMap = mapData;

        m_MapImage.LoadImageFile(0, mapData.imagePath);
        m_MapTitle.SetText(mapData.name);
        m_MapDescription.SetText(mapData.description);

        string statsText = "";
        if (mapData.status == "Aberto")
        {
            statsText += "<p>Jogadores Online: " + mapData.playersOnline + "</p>";
            statsText += "<p>Tempo Restante: " + mapData.timeRemaining + "</p>";
            statsText += "<p>Nível de Loot: " + mapData.levelOfLoot + "</p>";
            m_ActionButton.SetText("INGRESSAR");
            m_ActionButton.SetColor(ARGB(255, 0, 255, 0)); // Verde Neon
        }
        else
        {
            statsText += "<p>Status: " + mapData.status + "</p>";
            statsText += "<p>Tempo para Abrir: " + mapData.timeToOpen + "</p>";
            statsText += "<p>Jogadores na Fila: " + mapData.playersInQueue + "</p>";
            m_ActionButton.SetText("INGRESSAR NA FILA");
            m_ActionButton.SetColor(ARGB(255, 255, 165, 0)); // Amarelo/Âmbar
        }
        m_MapStats.SetText(statsText);
    }

    override bool OnClick(Widget w, int x, int y, int button)
    {
        super.OnClick(w, x, y, button);

        if (w == m_CloseButton)
        {
            GetGame().GetUIManager().HideScriptedMenu(this);
            return true;
        }

        if (w == m_ActionButton)
        {
            if (m_SelectedMap)
            {
                if (m_SelectedMap.status == "Aberto") 
                {
                    // Grita para o servidor no canal 54321 que queremos entrar!
                    GetGame().RPCSingleParam(GetGame().GetPlayer(), 54321, new Param1<string>(m_SelectedMap.id), true);
                    GetGame().GetUIManager().HideScriptedMenu(this);
                }
                else
                {
                    Print("[PNH_UI] Mapa fechado. Sistema de fila será ativado na próxima fase.");
                }
            }
            return true;
        }

        // Lógica para clicar em um item da lista de mapas
        PNH_MapData clickedMapData;
        if (w.GetUserData(clickedMapData))
        {
            UpdateMapDetails(clickedMapData);
            return true;
        }

        return false;
    }
}