class PNH_SafeZoneManager
{
    // Coordenadas exatas que você enviou
    private static vector m_HubCenter = "2669.597900 10.569989 1310.054810"; 
    private static float m_HubRadius = 600.0; // 600 metros de raio

    // Retorna verdadeiro se a posição do jogador estiver dentro do círculo de 600m
    static bool IsInHub(vector pos)
    {
        // Calcula a distância 3D entre o jogador e o centro da Prisão
        if (vector.Distance(pos, m_HubCenter) <= m_HubRadius)
        {
            return true;
        }
        return false;
    }
}