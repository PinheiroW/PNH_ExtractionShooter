class PNH_MapSelectionMenu extends UIScriptedMenu
{
    protected TextWidget m_OperatorName, m_ATMBalance, m_MapTitle;
    protected ButtonWidget m_CloseButton, m_ActionButton;
    protected GridSpacerWidget m_MapGridSpacer;
    protected ImageWidget m_MapImage;
    protected RichTextWidget m_MapDescription;
    protected ref PNH_MapConfig m_MapConfig;
    protected ref PNH_MapData m_SelectedMap;

    override Widget Init()
    {
        // Certifique-se que o caminho no PBO e exatamente este:
        layoutRoot = GetGame().GetWorkspace().CreateWidgets("PNH_ExtractionShooter/GUI/layouts/PNH_MapSelectionMenu.layout");
        
        if (!layoutRoot) return null; // Trava contra crash 0x10

        m_OperatorName = TextWidget.Cast(layoutRoot.FindAnyWidget("PNH_OperatorName"));
        m_ATMBalance = TextWidget.Cast(layoutRoot.FindAnyWidget("PNH_ATMBalance"));
        m_CloseButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("PNH_CloseButton"));
        m_MapGridSpacer = GridSpacerWidget.Cast(layoutRoot.FindAnyWidget("PNH_MapGridSpacer"));
        m_MapImage = ImageWidget.Cast(layoutRoot.FindAnyWidget("PNH_MapImage"));
        m_MapTitle = TextWidget.Cast(layoutRoot.FindAnyWidget("PNH_MapTitle"));
        m_MapDescription = RichTextWidget.Cast(layoutRoot.FindAnyWidget("PNH_MapDescription"));
        m_ActionButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("PNH_ActionButton"));

        return layoutRoot;
    }

    override void OnShow()
    {
        super.OnShow();
        GetGame().GetUIManager().ShowUICursor(true);
        GetGame().GetMission().PlayerControlDisable(INPUT_EXCLUDE_ALL);

        m_MapConfig = PNH_MapConfig.LoadConfig("$profile:PNH_ExtractionShooter/PNH_MapConfig.json");
        PopulateMapList();

        string name;
        GetGame().GetPlayerName(name);
        if (m_OperatorName) m_OperatorName.SetText("OPERADOR: " + name);
        
        if (m_MapConfig && m_MapConfig.maps.Count() > 0)
            UpdateMapDetails(m_MapConfig.maps.Get(0));
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
            ref PNH_MapData data = m_MapConfig.maps.Get(i);
            Widget item = GetGame().GetWorkspace().CreateWidgets("PNH_ExtractionShooter/GUI/layouts/PNH_MapListItem.layout", m_MapGridSpacer);
            if (item)
            {
                TextWidget t = TextWidget.Cast(item.FindAnyWidget("PNH_MapListItemName"));
                if (t) t.SetText(data.name);
                item.SetUserData(data);
            }
        }
    }

    protected void UpdateMapDetails(PNH_MapData data)
    {
        if (!data || !m_MapImage) return;
        m_SelectedMap = data;
        if (m_MapTitle) m_MapTitle.SetText(data.name);
        if (m_MapDescription) m_MapDescription.SetText(data.description);
        
        string tex = "PNH_ExtractionShooter/GUI/textures/PNH_NWAF_Large_P2.edds";
        if (data.id == "Tisy") tex = "PNH_ExtractionShooter/GUI/textures/PNH_Tisy_Large_P2.edds";
        m_MapImage.LoadImageFile(0, tex);
    }

    override void OnHide()
    {
        super.OnHide();
        GetGame().GetUIManager().ShowUICursor(false);
        GetGame().GetMission().PlayerControlEnable(false);
    }

    override bool OnClick(Widget w, int x, int y, int button)
    {
        if (w == m_CloseButton) { GetGame().GetUIManager().HideScriptedMenu(this); return true; }
        if (w == m_ActionButton && m_SelectedMap)
        {
            GetGame().RPCSingleParam(GetGame().GetPlayer(), 54321, new Param1<string>(m_SelectedMap.id), true);
            GetGame().GetUIManager().HideScriptedMenu(this);
            return true;
        }
        return false;
    }
}