using TMPro;
using UnityEngine;
using System.Collections.Generic;

public class Splitter : MonoBehaviour
{
    
    [SerializeField]
    private GameObject mediumEnemy;
    [SerializeField]
    private GameObject smallEnemy;
    [SerializeField]
    float horizontalSplitIntensity = 0;
    [SerializeField]
    float verticalSplitIntensity = 0;

    public int totalEnemyCount { get; private set; } = 0;

    public void Start()
    {
        List<CollisionHandlerEnemy> initialEnemyCount = new List<CollisionHandlerEnemy>(FindObjectsByType<CollisionHandlerEnemy>(FindObjectsSortMode.None));
        totalEnemyCount = initialEnemyCount.Count; //storing enemies count (game manager?)
    }

    public void split( GameObject hitSphere)
    {
        Vector3 pos = hitSphere.transform.position ;
        Quaternion rot = hitSphere.transform.rotation;
        Vector3 sphereDirection = hitSphere.GetComponent<CollisionHandlerEnemy>().direction;

        //checking "size" and ahndling enemy count
        if (hitSphere.tag == "BigEnemy")
        {
            splitHandler(mediumEnemy,in pos,in rot, sphereDirection);
            Destroy(hitSphere);
            totalEnemyCount++;

        }
        else if (hitSphere.tag == "MediumEnemy")
        {
            splitHandler(smallEnemy, in pos, in rot, sphereDirection);
            Destroy(hitSphere);
            totalEnemyCount++;

        }
        else if (hitSphere.tag == "SmallEnemy")
        {
            Destroy(hitSphere);
            totalEnemyCount--;
        }
    }
    void splitHandler(GameObject sphereSize, in Vector3 pos, in Quaternion rot,  Vector3 lastDirection)
    {

        //creating 2 sphere instances, 
        GameObject newSphere1 = Instantiate(sphereSize, pos, rot);
        GameObject newSphere2 = Instantiate(sphereSize, pos, rot);

        //getting their RidigBody
        Rigidbody rb1 = newSphere1.GetComponent<Rigidbody>();
       Rigidbody rb2 = newSphere2.GetComponent<Rigidbody>();

        // to add initial forces and setting their direction correctly
        rb1.AddForce((lastDirection * -horizontalSplitIntensity) + Vector3.up * verticalSplitIntensity, ForceMode.Force);
        newSphere1.GetComponent<CollisionHandlerEnemy>().direction = lastDirection;

        rb2.AddForce((lastDirection * horizontalSplitIntensity) + Vector3.up * verticalSplitIntensity, ForceMode.Force);
        newSphere2.GetComponent<CollisionHandlerEnemy>().direction= -lastDirection;
        



    }
}
    

   
  



