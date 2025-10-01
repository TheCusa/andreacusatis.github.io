using System;
using System.Collections;
using System.Runtime.CompilerServices;
using UnityEngine;

public class Spawner : MonoBehaviour
{
    // Start is called once before the first execution of Update after the MonoBehaviour is created
    [SerializeField]
    public GameObject prefab;
    [SerializeField]
    float spawnTimer;




    private void Start()
    {
        StartCoroutine(spawnEnemy()); 
    }
    // Update is called once per frame
    IEnumerator spawnEnemy()
    {
        while (true)
        {
            Vector3 spawn = new Vector3(UnityEngine.Random.Range(-9, 9), UnityEngine.Random.Range(8, 11), 0);
            Instantiate(prefab, spawn, Quaternion.identity);
            yield return new WaitForSeconds(spawnTimer);
        }   
    }
}
