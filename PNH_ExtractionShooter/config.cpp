class CfgPatches
{
    class PNH_ExtractionShooter
    {
        units[]={};
        weapons[]={};
        requiredAddons[]=
        {
            "DZ_Data",
            "DZ_Scripts",
            "DZ_Gear_Consumables" 
        };
    };
};

class CfgVehicles
{
    class Paper; 

    class PNH_AccessCard_NWAF: Paper 
    {
        scope=2; 
        displayName="Cartão de Acesso - NWAF";
        descriptionShort="Um cartão de segurança militar de uso único. Permite a infiltração direta no Airfield (NWAF). O sistema de transporte consome o cartão após o uso.";
        itemSize[]={1,1}; 
        weight=10;
    };
};

class CfgMods
{
    class PNH_ExtractionShooter
    {
        dir = "PNH_ExtractionShooter";
        picture = "";
        action = "";
        hideName = 1;
        hidePicture = 1;
        name = "PNH Extraction Shooter";
        credits = "Pinheiro";
        author = "Pinheiro";
        authorID = "0"; 
        version = "0.1";
        extra = 0;
        type = "mod";
        
        dependencies[] = { "Game", "World", "Mission" };
        
        class defs
        {
            class gameScriptModule
            {
                value = "";
                files[] = { "PNH_ExtractionShooter/scripts/3_Game" };
            };
            class worldScriptModule
            {
                value = "";
                files[] = { "PNH_ExtractionShooter/scripts/4_World" };
            };
            class missionScriptModule
            {
                value = "";
                files[] = { "PNH_ExtractionShooter/scripts/5_Mission" };
            };
        };
    };
};