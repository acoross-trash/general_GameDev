using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class CCharacter
{
	string m_name;
	int m_nId;
	int m_nType;
	
	int m_nMaxHP;
	int m_nHP;
	
	public CCharacter(string name, int Id, int Type, int MaxHP)
	{
		m_name = name;
		m_nId = Id;
		m_nType = Type;
		m_nMaxHP = MaxHP;
		m_nHP = MaxHP;
	}
}

public class BattleStateBase
{
	static BattleStateBase m_instance;
	public GameManager m_Owner;
	
	public BattleStateBase(GameManager owner)
	{
		m_Owner = owner;
	}
	
	virtual public void OnStart(){}
	virtual public void OnTick(){}
	virtual public void OnEnd(){}
	virtual public void OnUpdate(){}
	virtual public void OnGUI(){}
}

public class InitBattleState : BattleStateBase
{
	public InitBattleState(GameManager owner)
		: base(owner)
	{
	}
	
	public override void OnStart ()
	{
		m_Owner.m_nPCs = new List<CCharacter>();
		m_Owner.m_nPCs.Add (new CCharacter("Hero", 0, 0, 10));
		m_Owner.m_nPCs.Add (new CCharacter("Mage", 1, 0, 10));
		m_Owner.m_nPCs.Add (new CCharacter("Clarric", 2, 0, 10));
		
		m_Owner.ChangeStateTo(new SelectCmdState(m_Owner));
	}
}

public class SelectCmdState : BattleStateBase
{
	public SelectCmdState(GameManager owner)
		: base(owner)
	{
	}

	public override void OnGUI()
	{
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

		// Attack, Defend, Skill, Item 버튼을 표시.

		Rect rect1 = new Rect(0, 0, 100, 50);
		Rect rect2 = new Rect(0, 50, 100, 50);
		Rect rect3 = new Rect(0, 100, 100, 50);
		Rect rect4 = new Rect(0, 150, 100, 50);

		bool ret1 = m_Owner.ShowButton(rect1, "Attack");
		bool ret2 = m_Owner.ShowButton(rect2, "Defend");
		bool ret3 = m_Owner.ShowButton(rect3, "Skill");
		bool ret4 = m_Owner.ShowButton(rect4, "Item");

		int idx = -1;
		if (ret1)
		{
			idx = 1;
		}
		else if (ret2)
		{
			idx = 2;
		}
		else if (ret3)
		{
			idx = 3;
		}
		else if (ret4)
		{
			idx = 4;
		}

		if (idx > -1)
		{
			m_Owner.ShowInfo("button " + idx.ToString() + " Clicked\n" + m_Owner.GetNameOfFocusedControl());
		}
	}
}

public class ExecCmdState : BattleStateBase
{
	public ExecCmdState(GameManager owner)
		: base (owner)
	{
	}
}

public class EndBattleState : BattleStateBase
{
	public EndBattleState(GameManager owner)
		: base (owner)
	{
	}
}