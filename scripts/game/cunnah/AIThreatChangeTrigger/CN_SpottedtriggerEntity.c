[EntityEditorProps(category: "GameScripted/Triggers", description: "The CN_AIStateChangeTrigger is a custom trigger designed to be used with the TILW_MissionFrameworkEntity.")]
class CN_SpottedTriggerEntityClass : TILW_BaseTriggerEntityClass
{
}

class CN_SpottedTriggerEntity : TILW_BaseTriggerEntity
{
	[Attribute("", UIWidgets.Auto, "Faction that can trigger this.", category: "Trigger Filter")]
	protected string m_ownerFactionKey;
	
	[Attribute("", UIWidgets.ResourceAssignArray, "Any of these faction keys will be completely ignored by the trigger.", category: "Trigger Filter")]
	protected ref array<string> m_ignoredFactions;
	
	protected ref array<SCR_ChimeraCharacter> m_alerted = {};
	protected bool m_hasTarget = false;
	
	override bool EvaluateCondition()
	{	
		bool current = m_hasTarget;
		m_hasTarget = false;
		
		Print(current);
		
		return current;
	}
	
	override bool TotalFilter(SCR_ChimeraCharacter cc)
	{
		if (!cc)
			return false;
		if (IsEntityDestroyed(cc))
			return false;
		if (!IsWithinShape(cc.GetOrigin()))
			return false;
		if (EntityUtils.IsPlayer(cc))
			return false; //no idea how player character responds and don't need this.
		FactionKey fkey = GetFactionKey(cc);
		if (fkey == "" || m_ignoredFactions.Contains(fkey))
			return false; // Spectator, or ignored faction
		return true;
	}
	
	override bool SpecialFilter(SCR_ChimeraCharacter cc)
	{
		FactionKey fKey = GetFactionKey(cc);
		if (GetFactionKey(cc) != m_ownerFactionKey)
			return false;
		
		AIAgent agent = cc.PS_GetAIAgent();
		if (!agent)
			return false;
		
		SCR_ChimeraAIAgent cAgent = SCR_ChimeraAIAgent.Cast(agent);
		if (!cAgent)
			return false;
		
		SCR_AIInfoComponent info = cAgent.m_InfoComponent;
		if (!info)
			return false;
		
		if (info.m_Perception.GetTargetCount(ETargetCategory.ENEMY) > 0)
			m_hasTarget = true;
		
		return true;
	}
}