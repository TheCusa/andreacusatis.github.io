using UnityEngine;

[CreateAssetMenu(fileName = "GameStats", menuName = "Scriptable Objects/GameStats")]
public class GameStats : ScriptableObject
{
    public int enemyDamage { get; }  = 1;
    public int playerHealth { get;  } = 3;
    public float playerMoveSpeed { get; } = 5;

}
