// PNH_MapSelectionMenu.c
// Gerencia a interface de seleção de mapas para o mod PNH_ExtractionShooter.

class PNH_MapSelectionMenu extends UIScriptedMenu
{
    protected Widget m_RootWidget;
    protected TextWidget m_OperatorName;
    protected TextWidget m_ATMBalance;
    protected ButtonWidget m_CloseButton;

    protected GridSpacerWidget m_MapGridSpacer; 
    protected ImageWidget m_MapImage;
    protected TextWidget m_MapTitle;
    protected RichTextWidget m_MapStats;
    protected RichTextWidget m_MapDescription;
    protected ButtonWidget m_ActionButton;

    protected ref PNH_MapConfig m_MapConfig;
    protected ref PNH_MapData m_SelectedMap;

    void PNH_MapSelectionMenu()
    {
    }

    override Widget Init()
    {
        // Caminho absoluto corrigido para o motor Enfusion
        m_RootWidget = GetGame().GetWorkspace().CreateWidgets("PNH_ExtractionShooter/GUI/layouts/PNH_MapSelectionMenu.layout");

        m_OperatorName = TextWidget.Cast(m_RootWidget.FindAnyWidget("PNH_OperatorName"));
        m_ATMBalance = TextWidget.Cast(m_RootWidget.FindAnyWidget("PNH_ATMBalance"));
        m_CloseButton = ButtonWidget.Cast(m_RootWidget.FindAnyWidget("PNH_CloseButton"));

        m_MapGridSpacer = GridSpacerWidget.Cast(m_RootWidget.FindAnyWidget("PNH_MapGridSpacer")); 
        m_MapImage = ImageWidget.Cast(m_RootWidget.FindAnyWidget("PNH_MapImage"));
        m_MapTitle = TextWidget.Cast(m_RootWidget.FindAnyWidget("PNH_MapTitle"));
        m_MapStats = RichTextWidget.Cast(m_RootWidget.FindAnyWidget("PNH_MapStats"));
        m_MapDescription = RichTextWidget.Cast(m_RootWidget.FindAnyWidget("PNH_MapDescription"));
        m_ActionButton = ButtonWidget.Cast(m_RootWidget.FindAnyWidget("PNH_ActionButton"));

        // Carrega a configuração (com trava de segurança para o cliente)
        m_MapConfig = PNH_MapConfig.LoadConfig("$profile:PNH/PNH_MapConfig.json"); 

        PopulateMapList();

        return m_RootWidget;
    }

    override void OnShow()
    {
        super.OnShow();
        GetGame().GetInput().ChangeGameFocus(1);
        GetGame().GetUIManager().ShowCursor(true);

        // Verifica identidade de forma segura para evitar crashes no OnShow
        if (GetGame().GetPlayer0() && GetGame().GetPlayer0().GetIdentity())
        {
            m_OperatorName.SetText("OPERADOR: " + GetGame().GetPlayer0().GetIdentity().GetName());
        }
        else 
        {
            m_OperatorName.SetText("OPERADOR: DESCONHECIDO");
        }
        
        m_ATMBalance.SetText("SALDO ATM: $0");

        if (m_MapConfig && m_MapConfig.maps.Count() > 0)
        {
            UpdateMapDetails(m_MapConfig.maps.Get(0));
        }
    }

    override void OnHide()
    {
        super.OnHide();
        GetGame().GetInput().ChangeGameFocus(-1);
        GetGame().GetUIManager().ShowCursor(false);
    }

    protected void PopulateMapList()
    {
        if (!m_MapGridSpacer || !m_MapConfig) return;

        // Limpeza correta de widgets filhos
        Widget child = m_MapGridSpacer.GetChildren();
        while (child)
        {
            Widget next = child.GetSibling();
            child.Unlink();
            child = next;
        }

        for (int i = 0; i < m_MapConfig.maps.Count(); i++)
        {
            ref PNH_MapData mapData = m_MapConfig.maps.Get(i);

            Widget mapItem = GetGame().GetWorkspace().CreateWidgets("PNH_ExtractionShooter/GUI/layouts/PNH_MapListItem.layout", m_MapGridSpacer); 
            TextWidget mapName = TextWidget.Cast(mapItem.FindAnyWidget("PNH_MapListItemName"));
            ImageWidget mapThumbnail = ImageWidget.Cast(mapItem.FindAnyWidget("PNH_MapListItemThumbnail"));

            if (mapName) mapName.SetText(mapData.name);
            if (mapThumbnail && mapData.imagePath != "") mapThumbnail.LoadImageFile(0, mapData.imagePath);

            mapItem.SetUserData(mapData); 
        }
    }

    protected void UpdateMapDetails(PNH_MapData mapData)
    {
        if (!mapData) return;

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
            m_ActionButton.SetColor(ARGB(255, 0, 255, 0));
        }
        else
        {
            statsText += "<p>Status: " + mapData.status + "</p>";
            statsText += "<p>Tempo para Abrir: " + mapData.timeToOpen + "</p>";
            statsText += "<p>Jogadores na Fila: " + mapData.playersInQueue + "</p>";
            m_ActionButton.SetText("INGRESSAR NA FILA");
            m_ActionButton.SetColor(ARGB(255, 255, 165, 0));
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

        if (w == m_ActionButton && m_SelectedMap)
        {
            if (m_SelectedMap.status == "Aberto") 
            {
                // Envia RPC 54321 para o servidor processar a entrada
                GetGame().RPCSingleParam(GetGame().GetPlayer(), 54321, new Param1<string>(m_SelectedMap.id), true);
                GetGame().GetUIManager().HideScriptedMenu(this);
            }
            return true;
        }

        Widget currentWidget = w;
        PNH_MapData clickedMapData;
        
        while (currentWidget)
        {
            // Separação necessária pois GetUserData retorna void no DayZ
            currentWidget.GetUserData(clickedMapData);
            if (clickedMapData)
            {
                UpdateMapDetails(clickedMapData);
                return true;
            }
            currentWidget = currentWidget.GetParent();
        }

        return false;
    }
}