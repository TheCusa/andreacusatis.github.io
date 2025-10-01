using UnityEngine;

public class SafeAreaManager : MonoBehaviour
{
    // Start is called once before the first execution of Update after the MonoBehaviour is created
    private RectTransform rt;

    private void Awake()
    {
        rt= GetComponent<RectTransform>();
        updateSafeArea();
    }
    //Dynamic safe area Resizing
    private void updateSafeArea()
    {
        int width = Screen.width;
        int height = Screen.height;

        Rect safeAreaRect = Screen.safeArea;
        float normalizedSafeAreaWidth = safeAreaRect.width / width;
        float normalizedSafeAreaHeight= safeAreaRect.height / height;

        float left = safeAreaRect.xMin / width;
        float right = 1f - (1f- left -  normalizedSafeAreaWidth);

        float bottom = safeAreaRect.yMin / height;
        float top = 1f - (1f - bottom - normalizedSafeAreaHeight);

        rt.anchorMin = new Vector2(left, bottom);
        rt.anchorMax = new Vector2(right, top);
        rt.sizeDelta = Vector2.zero;
    }
}
