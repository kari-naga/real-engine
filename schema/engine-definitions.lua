---@meta


---Primary component class. Inherit from this class to create components
---@class Component
---@field OnStart? fun(self: Component)
---@field OnUpdate? fun(self: Component)
---@field OnLateUpdate? fun(self: Component)
---@field OnDestroy? fun(self: Component)
---@field OnCollisionEnter? fun(self: Component, collision: Collision)
---@field OnCollisionExit? fun(self: Component, collision: Collision)
---@field OnTriggerEnter? fun(self: Component, collision: Collision)
---@field OnTriggerExit? fun(self: Component, collision: Collision)
---@field key string
---@field enabled boolean
---@field actor Actor


---Alternative 2D vector representation lacking methods and helpers
---@class vec2
---@field x number
---@field y number


---Primary 2D vector representation
---@class Vector2
---@field x number
---@field y number
---@operator add(Vector2): Vector2
---@operator sub(Vector2): Vector2
---@operator mul(number): Vector2
Vector2 = {}

---@param x number
---@param y number
---@return Vector2
function Vector2(x, y) end

---Normalize this vector and return its length
---@return number
function Vector2:Normalize() end

---@return number
function Vector2:Length() end

---@param a Vector2
---@param b Vector2
---@return Vector2
function Vector2.Distance(a, b) end

---@param a Vector2
---@param b Vector2
---@return Vector2
function Vector2.Dot(a, b) end


---@class Rigidbody: Component
---@field type string
---@field key string
---@field actor Actor
---@field enabled boolean
---@field x number
---@field y number
---@field body_type "dynamic" | "static" | "kinematic"
---@field precise boolean
---@field gravity_scale number
---@field density number
---@field angular_friction number
---@field rotation number
---@field has_collider boolean
---@field has_trigger boolean
---@field collider_type "box" | "circle"
---@field width number
---@field height number
---@field radius number
---@field friction number
---@field bounciness number
---@field trigger_type "box" | "circle"
---@field trigger_width number
---@field trigger_height number
---@field trigger_radius number
local Rigidbody = {}

---@return Vector2
function Rigidbody:GetPosition() end

---Returns the rotation in degrees clockwise
---@return number
function Rigidbody:GetRotation() end

---@return Vector2
function Rigidbody:GetVelocity() end

---Returns the angular velocity in degrees clockwise
---@return number
function Rigidbody:GetAngularVelocity() end

---@return number
function Rigidbody:GetGravityScale() end

---@return Vector2
function Rigidbody:GetUpDirection() end

---@return Vector2
function Rigidbody:GetRightDirection() end

---Applies a gradual force to the body
---@param force Vector2
function Rigidbody:AddForce(force) end

---Applies a gradual torque to the body
---@param torque number
function Rigidbody:AddTorque(torque) end

---Applies an instantaneous force to the body
---@param impulse Vector2
function Rigidbody:AddLinearImpulse(impulse) end

---Applies an instantaneous torque to the body
---@param impulse number
function Rigidbody:AddAngularImpulse(impulse) end

---@param velocity Vector2
function Rigidbody:SetVelocity(velocity) end

---@param position Vector2
function Rigidbody:SetPosition(position) end

---Sets the rotation in degrees clockwise
---@param rotation number
function Rigidbody:SetRotation(rotation) end

---Sets the angular velocity in degrees clockwise
---@param angular_velocity number
function Rigidbody:SetAngularVelocity(angular_velocity) end

---@param gravity_scale number
function Rigidbody:SetGravityScale(gravity_scale) end

---@param up_direction Vector2
function Rigidbody:SetUpDirection(up_direction) end

---@param right_direction Vector2
function Rigidbody:SetRightDirection(right_direction) end


---@class Collision
---@field other Actor
---@field point Vector2
---Relative velocity between this actor and the other actor (direction unspecified)
---@field relative_velocity number
---Vector pointing from this actor to the other actor (direction unspecified)
---@field normal Vector2


---@class HitResult
---@field actor Actor
---@field point Vector2
---Normal vector at the point of intersection
---@field normal Vector2
---@field is_trigger boolean


---@class Debug
Debug = {}

---@param msg string
function Debug.Log(msg) end

---@param msg string
function Debug.LogError(msg) end


---@class Actor
Actor = {}

---@return string
function Actor:GetName() end

---@return number
function Actor:GetID() end

---@param key string
---@return Component | nil
function Actor:GetComponentByKey(key) end

---@param type string
---@return Component | nil
function Actor:GetComponent(type) end

---@param type string
---@return Component[]
function Actor:GetComponents(type) end

---@param type string
function Actor:AddComponent(type) end

---@param component Component
function Actor:RemoveComponent(component) end

---@param name string
---@return Actor | nil
function Actor.Find(name) end

---@param name string
---@return Actor[]
function Actor.FindAll(name) end

---@param template_name string
---@return Actor
function Actor.Instantiate(template_name) end

---@param actor Actor
function Actor.Destroy(actor) end


---@class Application
Application = {}

---@return number
function Application.GetFrame() end

function Application.Quit() end

---@param ms number
function Application.Sleep(ms) end

---@param url string
function Application.OpenURL(url) end


---@alias Keycode "up" | "down" | "right" | "left" | "escape" | "lshift" | "rshift" | "lcctrl" | "rctrl" | "lalt" | "ralt" | "tab" | "return" | "enter" | "backspace" | "delete" | "insert" | "space" | "a" | "b" | "c" | "d" | "e" | "f" | "g" | "h" | "i" | "j" | "k" | "l" | "m" | "n" | "o" | "p" | "q" | "r" | "s" | "t" | "u" | "v" | "w" | "x" | "y" | "z" | "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" | "/" | ";" | "=" | "-" | "." | "," | "[" | "]" | "\\" | "'"

---@class Input
Input = {}

---@param keycode Keycode
---@return boolean
function Input.GetKey(keycode) end

---@param keycode Keycode
---@return boolean
function Input.GetKeyDown(keycode) end

---@param keycode Keycode
---@return boolean
function Input.GetKeyUp(keycode) end

---Returns the mouse coordinates in screen space
---@return vec2
function Input.GetMousePostion() end

---@param button_number number
---@return boolean
function Input.GetMouseButton(button_number) end

---@param button_number number
---@return boolean
function Input.GetMouseButtonDown(button_number) end

---@param button_number number
---@return boolean
function Input.GetMouseButtonUp(button_number) end

---Returns the amount the mouse scrolled this frame
---@return number
function Input.GetMouseScrollDelta() end


---@class Text
Text = {}

---@param text string
---@param x number
---@param y number
---@param font_name string
---@param font_size number
---@param r number
---@param g number
---@param b number
---@param a number
function Text.Draw(text, x, y, font_name, font_size, r, g, b, a) end


---@class Audio
Audio = {}

---@param channel number
---@param audio_name string
---@param does_loop boolean
function Audio.PlayAudio(channel, audio_name, does_loop) end

---@param channel number
function Audio.HaltAudio(channel) end

---@param channel number
---@param volume number
function Audio.SetVolume(channel, volume) end


---@class Image
Image = {}

---@param image_name string
---@param x number
---@param y number
function Image.DrawUI(image_name, x, y) end

---@param image_name string
---@param x number
---@param y number
---@param rotation_degrees number
---@param scale_x number
---@param scale_y number
---@param pivot_x number
---@param pivot_y number
---@param r number
---@param g number
---@param b number
---@param a number
---@param sorting_order number
function Image.DrawUIEx(image_name, x, y, rotation_degrees, scale_x, scale_y, pivot_x, pivot_y, r, g, b, a, sorting_order) end

---@param image_name string
---@param x number
---@param y number
function Image.Draw(image_name, x, y) end

---@param image_name string
---@param x number
---@param y number
---@param rotation_degrees number
---@param scale_x number
---@param scale_y number
---@param pivot_x number
---@param pivot_y number
---@param r number
---@param g number
---@param b number
---@param a number
---@param sorting_order number
function Image.DrawEx(image_name, x, y, rotation_degrees, scale_x, scale_y, pivot_x, pivot_y, r, g, b, a, sorting_order) end

---@param tileset_name string
---@param tile_x number
---@param tile_y number
---@param tile_w number
---@param tile_h number
---@param x number
---@param y number
function Image.DrawTile(tileset_name, tile_x, tile_y, tile_w, tile_h, x, y) end

---@param tileset_name string
---@param tile_x number
---@param tile_y number
---@param tile_w number
---@param tile_h number
---@param x number
---@param y number
---@param w number
---@param h number
---@param rotation_degrees number
---@param scale_x number
---@param scale_y number
---@param pivot_x number
---@param pivot_y number
---@param r number
---@param g number
---@param b number
---@param a number
---@param sorting_order number
function Image.DrawTileEx(tileset_name, tile_x, tile_y, tile_w, tile_h, x, y, w, h, rotation_degrees, scale_x, scale_y, pivot_x, pivot_y, r, g, b, a, sorting_order) end


---@param x number
---@param y number
---@param r number
---@param g number
---@param b number
---@param a number
function Image.DrawPixel(x, y, r, g, b, a) end


---@class Camera
Camera = {}

---@param x number
---@param y number
function Camera.SetPosition(x, y) end

---@return number
function Camera.GetPositionX() end

---@return number
function Camera.GetPositionY() end

---@param zoom number
function Camera.SetZoom(zoom) end

---@return number
function Camera.GetZoom() end


---@class Scene
Scene = {}

---@param name string
function Scene.Load(name) end

---@return string
function Scene.GetCurrent() end

---@param actor Actor
function Scene.DontDestroy(actor) end


---@class Physics
Physics = {}

---@param position Vector2
---@param direction Vector2
---@param distance number
---@return HitResult
function Physics.Raycast(position, direction, distance) end

---@param position Vector2
---@param direction Vector2
---@param distance number
---@return HitResult[]
function Physics.RaycastAll(position, direction, distance) end


---@class Event
Event = {}

---@param event_type string
---@param event_object any
function Event.Publish(event_type, event_object) end

---@param event_type string
---@param component Component
---@param func fun(component: Component, event_object: any)
function Event.Subscribe(event_type, component, func) end

---@param event_type string
---@param component Component
---@param func fun(component: Component, event_object: any)
function Event.Unsubscribe(event_type, component, func) end


---@class Time
Time = {}

---@return number
function Time.DeltaTime() end

---@return number
function Time.UnscaledDeltaTime() end

---@return number
function Time.GetTime() end

---@return number
function Time.GetUnscaledTime() end

---@return number
function Time.GetTimeScale() end

---@param time_scale number
function Time.SetTimeScale(time_scale) end
