// PNH_MapSelectionMenu.c
// Versão de Alta Estabilidade - PNH Extraction Shooter

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

    void PNH_MapSelectionMenu() {}

    override Widget Init()
    {
        // Tenta carregar o layout principal
        m_RootWidget = GetGame().GetWorkspace().CreateWidgets("PNH_ExtractionShooter/GUI/layouts/PNH_MapSelectionMenu.layout");

        if (!m_RootWidget)
        {
            Print("[PNH_UI] ERRO: Layout principal não encontrado. Verifique o caminho da pasta GUI!");
            return null;
        }

        // Mapeamento com verificação de segurança (Cast)
        m_OperatorName = TextWidget.Cast(m_RootWidget.FindAnyWidget("PNH_OperatorName"));
        m_ATMBalance = TextWidget.Cast(m_RootWidget.FindAnyWidget("PNH_ATMBalance"));
        m_CloseButton = ButtonWidget.Cast(m_RootWidget.FindAnyWidget("PNH_CloseButton"));
        m_MapGridSpacer = GridSpacerWidget.Cast(m_RootWidget.FindAnyWidget("PNH_MapGridSpacer"));
        m_MapImage = ImageWidget.Cast(m_RootWidget.FindAnyWidget("PNH_MapImage"));
        m_MapTitle = TextWidget.Cast(m_RootWidget.FindAnyWidget("PNH_MapTitle"));
        m_MapStats = RichTextWidget.Cast(m_RootWidget.FindAnyWidget("PNH_MapStats"));
        m_MapDescription = RichTextWidget.Cast(m_RootWidget.FindAnyWidget("PNH_MapDescription"));
        m_ActionButton = ButtonWidget.Cast(m_RootWidget.FindAnyWidget("PNH_ActionButton"));

        m_MapConfig = PNH_MapConfig.LoadConfig("$profile:PNH/PNH_MapConfig.json"); 
        
        PopulateMapList();

        return m_RootWidget;
    }

    override void OnShow()
    {
        super.OnShow();
        GetGame().GetInput().ChangeGameFocus(1);
        GetGame().GetUIManager().ShowCursor(true);

        if (GetGame().GetPlayer() && GetGame().GetPlayer().GetIdentity() && m_OperatorName)
        {
            m_OperatorName.SetText("OPERADOR: " + GetGame().GetPlayer().GetIdentity().GetName());
        }
        
        if (m_ATMBalance) m_ATMBalance.SetText("SALDO ATM: $0");

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

        Widget child = m_MapGridSpacer.GetChildren();
        while (child)
        {
            Widget next = child.GetSibling();
            child.Unlink();
            child = next;
        }

        for (int i = 0; i < m_MapConfig.maps.Count(); i++)
        {
            // Tenta carregar o item da lista
            Widget mapItem = GetGame().GetWorkspace().CreateWidgets("PNH_ExtractionShooter/GUI/layouts/PNH_MapListItem.layout", m_MapGridSpacer); 
            if (!mapItem) continue;

            TextWidget mapName = TextWidget.Cast(mapItem.FindAnyWidget("PNH_MapListItemName"));
            if (mapName) mapName.SetText(m_MapConfig.maps.Get(i).name);

            mapItem.SetUserData(m_MapConfig.maps.Get(i)); 
        }
    }

    protected void UpdateMapDetails(PNH_MapData mapData)
    {
        if (!mapData) return;
        m_SelectedMap = mapData;

        if (m_MapTitle) m_MapTitle.SetText(mapData.name);
        if (m_MapDescription) m_MapDescription.SetText(mapData.description);
        
        if (m_MapStats)
        {
            string statusInfo = "Status: " + mapData.status;
            m_MapStats.SetText(statusInfo);
        }
    }

    override bool OnClick(Widget w, int x, int y, int button)
    {
        super.OnClick(w, x, y, button);

        if (m_CloseButton && w == m_CloseButton)
        {
            GetGame().GetUIManager().HideScriptedMenu(this);
            return true;
        }

        if (m_ActionButton && w == m_ActionButton && m_SelectedMap)
        {
            if (m_SelectedMap.status == "Aberto") 
            {
                GetGame().RPCSingleParam(GetGame().GetPlayer(), 54321, new Param1<string>(m_SelectedMap.id), true);
                GetGame().GetUIManager().HideScriptedMenu(this);
            }
            return true;
        }

        Widget currentWidget = w;
        PNH_MapData clickedMapData;
        while (currentWidget)
        {
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