class PNH_PlayerData
{
    string SteamID;           
    string ActiveRaidID;      
    bool PendingWipe;         
    vector LogoutPosition;    
    int ExtractionTimer;      
    int BattlePassXP;         
    int BankBalance;          

    void PNH_PlayerData(string id = "")
    {
        SteamID = id;
        ActiveRaidID = "";
        PendingWipe = false;
        LogoutPosition = "0 0 0";
        ExtractionTimer = 0;
        BattlePassXP = 0;
        BankBalance = 0;
    }
}