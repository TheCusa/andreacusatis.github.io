using UnityEngine;

public class ProjectileSpawner : MonoBehaviour
{
    [SerializeField]
    GameObject projectile;

    [SerializeField]
     PlayerController player;

    [SerializeField]
    float fireRate = 0.5f;

    float projSpeed = 15f;
    private float nextShot = 0f;
    float timeElapsed = 0f;

    
    void Update()
    {
         
         timeElapsed = Time.time;
            
        if (player.isShooting && timeElapsed >= nextShot)          //adding Animator Fire Rate to Time.time for handling fire rate (to do)
        {
            spawnProjectile();
            nextShot = timeElapsed + fireRate;
           
        }
    }
    void spawnProjectile()
    {
       GameObject proj= Instantiate(projectile, transform.position, Quaternion.identity);
       Rigidbody rb= proj.GetComponent<Rigidbody>();
       rb.linearVelocity = Vector3.up * projSpeed;
    }
}