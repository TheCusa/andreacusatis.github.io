using TMPro;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.UI;

public class UIHandler : MonoBehaviour
{
    [SerializeField]
    public Text healthText;
    [SerializeField]
    TextMeshProUGUI winText;
    [SerializeField]
    TextMeshProUGUI defeatText;
    [SerializeField]
    TextMeshProUGUI reloadingText;

    GameStatsHandler gameStatsHandler;
    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        gameStatsHandler = FindFirstObjectByType<GameStatsHandler>();
        healthText.text = gameStatsHandler.gameStats.playerHealth.ToString();

        winText.enabled = false;
        defeatText.enabled = false;
        reloadingText.enabled = false;
    }
    public void winUIHandler ()
    {
        winText.enabled = true;
        reloadingText.enabled = true;
    }
    public void defeatUIHandler ()
    {
        healthText.enabled = false;
        defeatText.enabled = true;
        reloadingText.enabled = true;
    }
}
