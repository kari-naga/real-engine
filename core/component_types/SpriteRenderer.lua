---@class SpriteRenderer: Component
SpriteRenderer = {
	sprite = "???",
	w = -1,
	h = -1,
	r = 255,
	g = 255,
	b = 255,
	a = 255,
	sorting_order = 0,
}

function SpriteRenderer:OnStart()
	self.pos = Vector2(0, 0)
	self.rot_degrees = 0
end

function SpriteRenderer:OnUpdate()
	self.rb = self.actor:GetComponent("Rigidbody") --[[@as Rigidbody]]

	if self.rb ~= nil then
		self.pos = self.rb:GetPosition()
		self.rot_degrees = self.rb:GetRotation()
	end

	Image.DrawEx(self.sprite, self.pos.x, self.pos.y, self.rot_degrees, 1.0, 1.0, 0.5, 0.5, self.r, self.g, self.b, self.a, self.sorting_order)
end
