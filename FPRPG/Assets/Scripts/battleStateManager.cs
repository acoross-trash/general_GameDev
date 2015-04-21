using UnityEngine;
using System.Collections;

public class battleStateManager : MonoBehaviour {

	public enum BattleState
	{
		Init,
		SelectCommands,
		ExecCommands,
		BattleEnd
	}
	
	public BattleState m_BattleState = BattleState.Init;

	// Use this for initialization
	void Start () 
	{
		m_BattleState = BattleState.Init;
	}
	
	// Update is called once per frame
	void Update () 
	{
	
	}
}
