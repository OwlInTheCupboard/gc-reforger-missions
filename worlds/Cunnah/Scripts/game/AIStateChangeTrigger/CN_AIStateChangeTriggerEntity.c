[EntityEditorProps(category: "GameScripted/Triggers", description: "The CN_AIStateChangeTrigger is a custom trigger designed to be used with the TILW_MissionFrameworkEntity.")]
class CN_AIThreatStateTriggerEntityClass : TILW_BaseTriggerEntityClass
{
}

class CN_AIThreatStateTriggerEntity : TILW_BaseTriggerEntity
{
	[Attribute("2", UIWidgets.ComboBox, "Minimum threat state to trigger", enums: ParamEnumArray.FromEnum(EAIThreatState), category: "Trigger Condition")]
	protected EAIThreatState m_minTriggerState;
	
	[Attribute("", UIWidgets.Auto, "Which faction can trigger this?", category: "Trigger Condition")]
	protected string m_ownerFactionKey;
	
	[Attribute("", UIWidgets.ResourceAssignArray, "Any of these faction keys will be completely ignored by the trigger.", category: "Trigger Filter")]
	protected ref array<string> m_ignoredFactions;
	
	override bool EvaluateCondition()
	{
		if (m_specialCount <= 0)
			return false;
		
		return true;
	}
	
	override bool TotalFilter(SCR_ChimeraCharacter cc)
	{
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
		
		SCR_AIThreatSystem threatSystem = info.GetThreatSystem();
		if (!threatSystem)
			return false;
		
		EAIThreatState state = threatSystem.GetState();
		if (state < m_minTriggerState)
			return false;
		
		return true;
	}
}