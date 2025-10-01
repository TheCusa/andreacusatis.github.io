using System.Collections;
using UnityEngine;

public class CollisionHandlerEnemy : MonoBehaviour
{
    Rigidbody rb;
    Splitter splitter;
    CollisionHandlerPlayer colHandlerPlayer;
    GameOverHandler gameOverHandler;
    // Start is called once before the first execution of Update after the MonoBehaviour is created

    Vector3 scaleFactor = new Vector3(0.5f, 0.5f, 0.5f);
    [SerializeField]
    float force = 600;
    [SerializeField]
    float sideForce = 80f;
    
    public Vector3 direction { get; set; } = Vector3.right;
    


    private void Start()
    {
        rb = GetComponent<Rigidbody>();
        splitter = FindFirstObjectByType<Splitter>();
        colHandlerPlayer = FindFirstObjectByType<CollisionHandlerPlayer>();
        gameOverHandler = FindFirstObjectByType<GameOverHandler>();
        int enemiesLayer = LayerMask.NameToLayer("Enemies");
        Physics.IgnoreLayerCollision(enemiesLayer, enemiesLayer, true);
    }
    private void OnCollisionEnter(Collision other)
    {
        ContactPoint contactPoints = other.contacts[0];
        Vector3 normal= contactPoints.normal;
        
        switch (other.gameObject.tag)
        {
            case ("Projectiles"):
                Destroy(other.gameObject);
                if (splitter != null)
                {
                    splitter.split(this.gameObject);                            //splitting the current object
                    gameOverHandler.gameOverHandling(); //checkin if game over conditions are met
                }
                else
                {
                    Debug.LogError("Splitter not found");

                }

                break;

            //Adding a constant up and right force
            case ("Environment"):

                rb.AddForce(normal * force, ForceMode.Force);
                rb.AddForce( direction * sideForce, ForceMode.Force);
                break;

            case ("Player"):
                if (splitter != null && colHandlerPlayer)
                {
                    splitter.split(this.gameObject);  
                }

                break;
            case ("Walls"):
                rb.AddForce(normal * (sideForce * 1.5f), ForceMode.Force);

                direction = -direction;
                break;

            default:
                break;
                
        }
    }
    
}
