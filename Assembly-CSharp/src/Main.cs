using System;

public class Main
{
    static int CSharpInit()
    {
        var gameOb = MonoEngine.GameObject.Find("Game");
        Console.WriteLine($"gameOb: {gameOb}, IsNull: {gameOb == null}");
        return 1;
    }
}