using UnityEngine;
using System.Collections;

[RequireComponent(typeof(Animator))]
public class EnemyEventHandler : MonoBehaviour {

	public GameObject lightningPrefab = null; 
	public Transform sfxSpawnTransform;

	private Animator enemyAnimator = null;

	private float m_nHitTime = 0.0f;

	// Use this for initialization
	void Start () {
		enemyAnimator = GetComponent<Animator>();
	}

	void OnEnable()
	{
		GameManager.onEnemyAttacked += OnEnemyAttackedHandler;
	}

	void OnDisable()
	{
		GameManager.onEnemyAttacked -= OnEnemyAttackedHandler;
	}

	void OnEnemyHitEnd()
	{
		//enemyAnimator.SetBool("bHit", false);
	}

	void OnEnemyAttackedHandler()
	{
		Debug.Log("Enemy Attacked");

		enemyAnimator.SetBool("bHit", true);
		m_nHitTime = Time.time + 0.4f;

		// 총알 오브젝트를 만든다
		GameObject newSFX = (GameObject)Instantiate(lightningPrefab);
		
		// 총알의 시작 위치를 설정한다
		newSFX.transform.position = sfxSpawnTransform.position;
		
		// 새 오브젝트의 PlayerBulletController 컴포넌트를 할당해서 몇 가지 데이터를 지정할 수 있다
		//PlayerBulletController bullCon = newBullet.GetComponent<PlayerBulletController>();
		SFXController lghtCon = newSFX.GetComponent<SFXController>();

		// 플레이어 오브젝트를 지정한다
		//bullCon.playerObject = gameObject;
		lghtCon.playerObject = gameObject;

		// 총알 발사!
		//bullCon.launchBullet();    
		lghtCon.LaunchSFX();
	}

	// Update is called once per frame
	void Update () {
		if (m_nHitTime > 0.0f)
		{
			if (m_nHitTime < Time.time)
			{
				enemyAnimator.SetBool("bHit", false);
			}
		}
	}
}
