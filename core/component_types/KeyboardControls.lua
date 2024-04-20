---@class KeyboardControls: Component
KeyboardControls = {
	speed = 5,
	jump_power = 350,
}

function KeyboardControls:OnStart()
	self.rb = self.actor:GetComponent("Rigidbody") --[[@as Rigidbody]]
end

function KeyboardControls:OnUpdate()
	-- Horizontal
	local horizontal_input = 0
	if Input.GetKey("right") then
		horizontal_input = self.speed
	end

	if Input.GetKey("left") then
		horizontal_input = -self.speed
	end

	-- Vertical
	local vertical_input = 0

	-- Check on ground
	local ground_object = Physics.Raycast(self.rb:GetPosition(), Vector2(0, 1), 1)
	if Input.GetKeyDown("up") or Input.GetKeyDown("space") then
		if ground_object ~= nil then
			vertical_input = -self.jump_power
		end
	end

	self.rb:AddForce(Vector2(horizontal_input, vertical_input))
end
