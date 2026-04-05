class PNH_HubCleanup
{
    static void RunCleanup()
    {
        vector hubCenter = "2669.597900 10.569989 1310.054810";
        float hubRadius = 600.0;

        array<Object> objectsInArea = new array<Object>;
        // Varre a área em busca de objetos
        GetGame().GetObjectsAtPosition(hubCenter, hubRadius, objectsInArea, null);

        int itemsDeleted = 0;

        foreach (Object obj : objectsInArea)
        {
            ItemBase item = ItemBase.Cast(obj);
            
            // Só continua se for um item (ignora jogadores, IAs, carros e prédios)
            if (item)
            {
                // Se o item NÃO tem um "Pai" (ou seja, não está na mão nem na mochila de ninguém)
                if (!item.GetHierarchyParent())
                {
                    item.Delete();
                    itemsDeleted++;
                }
            }
        }
        Print("[PNH_Cleanup] Varredura da Prisão concluída. Itens de lixo deletados: " + itemsDeleted);
    }
}