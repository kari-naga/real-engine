---@class TrackingCamera: Component
TrackingCamera = {
	ease_factor = 0.1,
}

function TrackingCamera:OnUpdate()
	local rb = self.actor:GetComponent("Rigidbody") --[[@as Rigidbody]]
	if rb == nil then
		return
	end
	local desired_position = rb:GetPosition()
	local current_position = Vector2(Camera.GetPositionX(), Camera.GetPositionY())

	local new_position = current_position + (desired_position - current_position) * self.ease_factor
	Camera.SetPosition(new_position.x, new_position.y)
end
