using UnrealBuildTool;

public class Weapon : ModuleRules
{
	public Weapon(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;


        PrivateIncludePaths.Add(ModuleDirectory);

        PublicDependencyModuleNames.Add("Core");
        PrivateDependencyModuleNames.Add("RPG");//프로젝트명

        PrivateDependencyModuleNames.Add("Projects");
        PrivateDependencyModuleNames.Add("InputCore");
        PrivateDependencyModuleNames.Add("EditorFramework");
        PrivateDependencyModuleNames.Add("ToolMenus");
		
        PrivateDependencyModuleNames.Add("CoreUObject");
        PrivateDependencyModuleNames.Add("Engine");
        PrivateDependencyModuleNames.Add("Slate");
        PrivateDependencyModuleNames.Add("SlateCore");

        PrivateDependencyModuleNames.Add("UnrealEd");//UFactory 

		//PrivateDependencyModuleNames.AddRange(
		//	new string[]
		//	{
		//		"Projects",
		//		"InputCore",
		//		"EditorFramework",
		//		"UnrealEd",
		//		"ToolMenus",
		//		"CoreUObject",
		//		"Engine",
		//		"Slate",
		//		"SlateCore",
		//		// ... add private dependencies that you statically link with here ...	
		//	}
		//	);
		
		
	}
}
