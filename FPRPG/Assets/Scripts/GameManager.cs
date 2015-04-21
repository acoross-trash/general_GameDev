using UnityEngine;
using System.Collections;
using UnityEngine.UI;
using System.Collections.Generic;

public class GameManager : MonoBehaviour 
{
#region Battle State
	private BattleStateBase m_BattleState;

	public void ChangeStateTo (BattleStateBase newState)
	{
		if (m_BattleState != null)
		{
			m_BattleState.OnEnd();
		}

		m_BattleState = newState;

		if (m_BattleState != null)
		{
			m_BattleState.OnStart();
		}
	}
#endregion

	public List<CCharacter> m_nPCs;

	public delegate void EnemyEventHandler();
	public static event EnemyEventHandler onEnemyAttacked;
	public Text infoText;

	// Use this for initialization
	void Start () 
	{
		Debug.Log ("GameManager Start");
		infoText.text = "Battle Start by GameManager::Start()";

		// 현재 스테이트를 설정.
		m_BattleState = new InitBattleState(this);
		m_BattleState.OnStart();
	}
	
	// Update is called once per frame
	void Update () 
	{
		#region SFX test code.
		/*bool bJump = Input.GetButtonDown("Jump");

		if (bJump == true)
		{
			Debug.Log ("Jump Clicked");
			if (onEnemyAttacked != null)
			{
				Debug.Log ("onEnemyAttacked call");
				onEnemyAttacked();
			}
		}*/
		#endregion
	}		

	void OnGUI()
	{
		#region Example code
		/*GUI.Button (new Rect(200, 100, 50, 50), "button2");

		GUI.SetNextControlName("attack button");
		//GUI.Box(new Rect(0, 0, 10, 10), "test ui");
		if (GUI.Button(new Rect(0, 0, 100, 100), "test button"))
		{
			infoText.text = "Button Clicked";

			if (onEnemyAttacked != null)
			{
				Debug.Log ("onEnemyAttacked call");
				onEnemyAttacked();
			}
		}

		GUI.FocusControl("attack button");*/
		#endregion

		m_BattleState.OnGUI();
	}

	public bool ShowButton(Rect rect, string text)
	{
		GUI.SetNextControlName(text);
		return GUI.Button (rect, text);
	}

	public void ShowInfo(string text)
	{
		infoText.text = text;
	}

	public string GetNameOfFocusedControl()
	{
		return GUI.GetNameOfFocusedControl();
	}
}