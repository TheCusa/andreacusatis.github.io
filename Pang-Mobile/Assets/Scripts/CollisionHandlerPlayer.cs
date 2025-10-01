using UnityEngine;
using System.Collections;
using System.Drawing;

public class CollisionHandlerPlayer : MonoBehaviour
{
    PlayerController pController;
    Rigidbody rb;
    GameOverHandler gameOverHandler;
    UIHandler uiRef;
    GameStatsHandler gsHandler;



    public int currentHealth { get; private set; } = 0;
    int playerLayer;
    int enemiesLayer;
    float invincibilityWindow = 1f;

    void Start()
    {
        pController = GetComponent<PlayerController>();
        rb = GetComponent<Rigidbody>();
        gameOverHandler = FindFirstObjectByType<GameOverHandler>();
        uiRef = FindFirstObjectByType<UIHandler>();
        //Better in a game manager?
        playerLayer = LayerMask.NameToLayer("Player");
        enemiesLayer = LayerMask.NameToLayer("Enemies");
        gsHandler = FindFirstObjectByType<GameStatsHandler>();
        currentHealth = gsHandler.gameStats.playerHealth;
    }

    private void OnTriggerEnter (Collider other)
    {
        switch (other.gameObject.tag)
        {
            case ("Ladder"):
                if (pController != null)
                {
                    pController.canClimb = true;
                    rb.useGravity = false;
                }
                break;
            default:
                break;
        }
        
    }
     private void OnTriggerExit(Collider other)
    {
        if (other.gameObject.tag == "Ladder")
        {
            pController.canClimb = false;
            rb.useGravity = true;
        }
    }
    private void OnCollisionEnter(Collision other)
    {
        if (other.gameObject.layer == enemiesLayer)
        {
            currentHealth -= gsHandler.gameStats.enemyDamage;
            uiRef.healthText.text = currentHealth.ToString();      //updating UI 
           if( isAlive())                                    //checking if player is still alive
            {
                StartCoroutine(invincibility());
            }
            gameOverHandler.gameOverHandling();          //checkin if game over conditions are met

        }
    }
    public  bool isAlive()
    {
        if(currentHealth <= 0)
        {
            handleDeath();
            return false;
        }
        return true;
    }
    public void handleDeath()
    {
        pController.animator.SetBool(pController.Animator_Dead, true);
        Destroy(gameObject, pController.animator.GetCurrentAnimatorStateInfo(0).length);
        pController.enabled = false;
    }
   public IEnumerator invincibility()
    {
        
        //Disabling collision with enemies' layer  and granting player a short immunity window to prevent double collision due to ball splitting
        Physics.IgnoreLayerCollision(playerLayer, enemiesLayer, true);
        yield return new WaitForSeconds(invincibilityWindow);
        Physics.IgnoreLayerCollision(playerLayer, enemiesLayer, false);

    }

}
