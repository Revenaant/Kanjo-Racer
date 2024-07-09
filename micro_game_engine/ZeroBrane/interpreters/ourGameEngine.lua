return {
  name = "ourGameEngine",
  description = "Very simple game engine, using lua",
  api = {"baselib", "ourGameEngine"},
  frun = function(self,wfilename,rundebug)
    local exe = self:fworkdir(wfilename)..'/mge'
    local cmd = nil
    if rundebug then
      ide:GetDebugger():SetOptions({{runstart = ide.config.debugger.runonstart == true}})
      cmd = '"'..exe..'" -debug'
    else 
      cmd = '"'..exe..'"'
    end
    
--  CommandLineRun(cmd,wdir,tooutput,nohide,stringcallback,uid,endcallback)    
    CommandLineRun(
      cmd, -- exe -debug
      self:fworkdir(wfilename), -- work
      true,
      false
    )
  end,
  hasdebugger = true,
}