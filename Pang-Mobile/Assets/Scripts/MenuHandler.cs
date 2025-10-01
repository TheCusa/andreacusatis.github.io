using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
public class MenuHandler : MonoBehaviour
{
    // Start is called once before the first execution of Update after the MonoBehaviour is created
    [SerializeField]
    Button playButton;
    [SerializeField]
    Button quitButton;

    //Main menu Handler
    private void OnEnable()
    {
        if(playButton != null)
        {
            playButton.onClick.AddListener(StartGame);

        }
        if(quitButton != null)
        {
            quitButton.onClick.AddListener(QuitApplication);
        }
    }
    private void OnDisable()
    {
        if (playButton != null)
        {
            playButton.onClick.RemoveListener(StartGame);

        }
        if (quitButton != null)
        {
            quitButton.onClick.RemoveListener(QuitApplication);
        }
    }

    public void StartGame()
    {
        SceneManager.LoadScene(1);
    }
    public void QuitApplication()
    {
        Application.Quit();
    }
}
