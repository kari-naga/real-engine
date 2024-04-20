---@class TileRenderer: Component
TileRenderer = {
	tileset = "???",
	x = 0,
	y = 0,
	w = 32,
	h = 32,
	tx = 0,
	ty = 0,
	tw = 32,
	th = 32,
	tfx = false,
	tfy = false,
	r = 255,
	g = 255,
	b = 255,
	a = 255,
	sorting_order = 0,
}

function TileRenderer:OnStart()
	self.pos = Vector2(self.x, self.y)
	self.rot_degrees = 0
end

function TileRenderer:OnUpdate()
	self.rb = self.actor:GetComponent("Rigidbody") --[[@as Rigidbody]]

	if self.rb ~= nil then
		self.pos = self.rb:GetPosition()
		self.rot_degrees = self.rb:GetRotation()
	end

	Image.DrawTileEx(self.tileset, self.tx, self.ty, self.tw, self.th, self.pos.x, self.pos.y, self.w, self.h, self.rot_degrees, (self.tfx and -1.0 or 1.0), (self.tfy and -1.0 or 1.0), 0.5, 0.5, self.r, self.g, self.b, self.a, self.sorting_order)
end
