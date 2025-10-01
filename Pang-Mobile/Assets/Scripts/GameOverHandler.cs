using UnityEngine;
using UnityEngine.Rendering;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using TMPro;


public class GameOverHandler : MonoBehaviour
{
    UIHandler uiHandler;
    Splitter splitter;
    CollisionHandlerPlayer collisionHandlerPlayer;
   
    // Start is called once before the first execution of Update after the MonoBehaviour is created
    private void Start()
    {
        splitter = FindFirstObjectByType<Splitter>();
        uiHandler = FindAnyObjectByType<UIHandler>();
       collisionHandlerPlayer = FindFirstObjectByType<CollisionHandlerPlayer>();
    }
    
   public void gameOverHandling( )
    {
        int health = collisionHandlerPlayer.currentHealth;
        //checking if all enemie are dead and also player's health is not 0
        if (splitter.totalEnemyCount == 0 && health >0)
        {
            uiHandler.winUIHandler();
            Invoke("loadMenu", 3);

        }
        else if (health <= 0)
        {
            uiHandler.defeatUIHandler();
            Invoke("loadMenu", 3);

        }


    }
    private void loadMenu()
    {
        SceneManager.LoadScene(0);
        
    }
}
