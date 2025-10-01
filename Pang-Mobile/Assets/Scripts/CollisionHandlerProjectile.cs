using UnityEngine;

public class CollisionHandler : MonoBehaviour
{
    // Start is called once before the first execution of Update after the MonoBehaviour is created
    
    
    private void OnCollisionEnter(Collision other)
    {
        switch (other.gameObject.tag)
        {
            case ("Environment"):
                Destroy(gameObject);

                break;
            
            default:
                break;
        }

    }
    
    
}
