Behaviour = class(AIBase)

function Behaviour:Init()
end

function Behaviour:Update()
end

function Behaviour:GameEnd()
end

function Behaviour:UnitCreated(unit)
end

function Behaviour:UnitBuilt(unit)
end

function Behaviour:UnitDead(unit)
end

function Behaviour:UnitDamaged(unit,attacker,damage)
end

function Behaviour:UnitIdle(unit)
end

function Behaviour:SetUnit(unit)
	if unit == nil then
		-- this should never happen, you don't add behaviours to units you either cant
		-- control/see or that don't exist. This was called incorrectly
		game:SendToConsole( "Warning: Shard Behaviour:SetUnit was called with a nil unit for "..self:Name() )
	end
	self.unit = unit
	self.engineID = unit.engineID
end

function Behaviour:SetAI(ai)
	self.ai = ai
	self.game = ai.game
	self.map = ai.map
end


function Behaviour:IsActive()
	return self.active
end

function Behaviour:Activate()
	--
end

function Behaviour:Deactivate()
	--
end

function Behaviour:Priority()
	return 0
end

function Behaviour:Passive()
	return false
end

function Behaviour:UnitMoveFailed(unit)
	self:UnitIdle(unit)
end

function Behaviour:OwnerDied()
	return
end

function Behaviour:Name()
	return "UnknownBehaviour"
end

function Behaviour:EchoDebug(inStr)
	game:SendToConsole(self:Name() .. ": " .. inStr)
end
