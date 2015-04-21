using UnityEngine;
using System.Collections;

public class SFXController : MonoBehaviour {

	public GameObject playerObject = null;

	private float selfDestructTimer = 0.0f;

	// Use this for initialization
	void Start () {
	}
	
	// Update is called once per frame
	void Update () {
		if(selfDestructTimer > 0.0f)
		{
			if(selfDestructTimer < Time.time)
			{
				playerObject.SendMessage("OnEnemyHitEnd");
				Destroy(gameObject);
			}
		}
	}

	public void LaunchSFX()
	{
		selfDestructTimer = Time.time + 1.0f;
	}
}
