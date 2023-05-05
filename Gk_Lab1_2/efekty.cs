//-----------------------------------------------//
using System;

namespace Graf_kom
{
    public partial class f_graf_kom
    {

        //------------------
        //------------------
        public void Efekt1()
        {
            //efekt: przewijanie poziome obrazu w kierunku lewej strony ekranu 

            if (p >= L) p = 0;

            for (int j = 1; j <= L; j++)
            {
                for (int i = 1 + p; i <= K; i++)
                    ReadPixel(i, j);
                for (int i = 1; i <= p; i++)
                    ReadPixel(i, j);
            }
        }

        //------------------
        //------------------
        public void Efekt2()
        {
            //efekt: zas³anianie pionowe obrazu w kierunku górnej krawêdzi ekranu

            if (p >= L) p = 0;

            for (int j = 1; j <= L - p; j++)
            {
                for (int i = 1; i <= K; i++)
                    ReadPixel(i, j);
            }
            for (int j = 1; j <= p; j++)
            {
                for (int i = 1; i <= K; i++)
                    ReadTlo(N);
            }
        }

        //------------------
        //------------------
        public void Efekt3()
        {
            //efekt: przesuwanie obrazu wzd³u¿ przek¹tnej ekranu w kierunku dolnego prawego wierzcho³ka

            if (p >= L) p = 0;

            for (int j = 1; j <= p; j++)
                for (int i = 1; i <= K; i++)
                    ReadTlo(N);
            for (int j = 1; j <= L - p; j++)
            {
                for (int i = 1; i <= p; i++)
                    ReadTlo(N);
                for (int i = 1; i <= K - p; i++)
                    ReadPixel(i, j);
            }
        }

        //------------------
        //------------------
        public void RGB_NA_HLS()
        {
            System.Drawing.Color pixel;
            double suma = 0, jasnosc, kontrast, l_hls;
            for (int j = 1; j <= L; j++)
                for (int i = 1; i <= K; i++)
                {
                    pixel = m_obraz_w_pamieci.GetPixel(i - 1, j - 1);
                    l_hls = (Math.Max(Math.Max(pixel.R, pixel.G), pixel.B) + Math.Min(Math.Min(pixel.R, pixel.G), pixel.B)) / 2;
                    pixel = System.Drawing.Color.FromArgb((int)l_hls, (int)l_hls, (int)l_hls);
                    suma += l_hls;
                    m_ekran.SetPixel(i - 1, j - 1, pixel);
                }
            jasnosc = suma / (L * K);
            jasnosc_value.Text = Math.Round(jasnosc / 255 * 100, 0) + "%";
            suma = 0;
            for (int j = 1; j <= L; j++)
                for (int i = 1; i <= K; i++)
                {
                    pixel = m_obraz_w_pamieci.GetPixel(i - 1, j - 1);
                    l_hls = (Math.Max(Math.Max(pixel.R, pixel.G), pixel.B) + Math.Min(Math.Min(pixel.R, pixel.G), pixel.B)) / 2;
                    suma += (l_hls - jasnosc) * (l_hls - jasnosc);
                }
            kontrast = Math.Sqrt(suma / (L * K));
            kontrast_value.Text = Math.Round(kontrast / (255 / 2) * 100, 0) + "%";
            SetBitMap(ref m_ekran);
        }

        //------------------
        //------------------
        public void RGB_NA_HSV()
        {
            System.Drawing.Color pixel;
            double suma = 0, jasnosc, kontrast, l_hsv;
            for (int j = 1; j <= L; j++)
                for (int i = 1; i <= K; i++)
                {
                    pixel = m_obraz_w_pamieci.GetPixel(i - 1, j - 1);
                    l_hsv = Math.Max(Math.Max(pixel.R, pixel.G), pixel.B);
                    pixel = System.Drawing.Color.FromArgb((int)l_hsv, (int)l_hsv, (int)l_hsv);
                    suma += l_hsv;
                    m_ekran.SetPixel(i - 1, j - 1, pixel);
                }
            jasnosc = suma / (L * K);
            jasnosc_value.Text = Math.Round(jasnosc / 255 * 100, 0) + "%";
            suma = 0;
            for (int j = 1; j <= L; j++)
                for (int i = 1; i <= K; i++)
                {
                    pixel = m_obraz_w_pamieci.GetPixel(i - 1, j - 1);
                    l_hsv = Math.Max(Math.Max(pixel.R, pixel.G), pixel.B);
                    suma += (l_hsv - jasnosc) * (l_hsv - jasnosc);
                }
            kontrast = Math.Sqrt(suma / (L * K));
            kontrast_value.Text = Math.Round(kontrast / (255 / 2) * 100, 0) + "%";
            SetBitMap(ref m_ekran);
        }

        //------------------
        //------------------
        public void AVG_RGB()
        {
            System.Drawing.Color pixel;
            double suma = 0, jasnosc, kontrast, avg;
            for (int j = 1; j <= L; j++)
                for (int i = 1; i <= K; i++)
                {
                    pixel = m_obraz_w_pamieci.GetPixel(i - 1, j - 1);
                    avg = (pixel.R + pixel.G + pixel.B) / 3;
                    pixel = System.Drawing.Color.FromArgb((int)avg, (int)avg, (int)avg);
                    suma += avg;
                    m_ekran.SetPixel(i - 1, j - 1, pixel);
                }
            jasnosc = suma / (L * K);
            jasnosc_value.Text = Math.Round(jasnosc / 255 * 100, 0) + "%";
            suma = 0;
            for (int j = 1; j <= L; j++)
                for (int i = 1; i <= K; i++)
                {
                    pixel = m_obraz_w_pamieci.GetPixel(i - 1, j - 1);
                    avg = (pixel.R + pixel.G + pixel.B) / 3;
                    suma += (avg - jasnosc) * (avg - jasnosc);

                }
            kontrast = Math.Sqrt(suma / (L * K));
            kontrast_value.Text = Math.Round(kontrast / (255 / 2) * 100, 0) + "%";
            SetBitMap(ref m_ekran);
        }

        //------------------
        //------------------
        public void Jasnosc_change()
        {
            System.Drawing.Color pixel;
            int scroll_val = hScrollBar1.Value, r, g, b;
            double suma = 0, jasnosc, kontrast, avg;
            for (int j = 1; j <= L; j++)
                for (int i = 1; i <= K; i++)
                {
                    pixel = m_obraz_w_pamieci.GetPixel(i - 1, j - 1);
                    r = pixel.R + scroll_val;
                    if (r > 255) r = 255;
                    else if (r < 0) r = 0;
                    g = pixel.G + scroll_val;
                    if (g > 255) g = 255;
                    else if (g < 0) g = 0;
                    b = pixel.B + scroll_val;
                    if (b > 255) b = 255;
                    else if (b < 0) b = 0;
                    pixel = System.Drawing.Color.FromArgb(r, g, b);
                    avg = (r + g + b) / 3;
                    suma += avg;
                    m_ekran.SetPixel(i - 1, j - 1, pixel);
                }
            jasnosc = suma / (L * K);
            jasnosc_value.Text = Math.Round(jasnosc / 255 * 100, 0) + "%";
            suma = 0;
            for (int j = 1; j <= L; j++)
                for (int i = 1; i <= K; i++)
                {
                    pixel = m_ekran.GetPixel(i - 1, j - 1);
                    avg = (pixel.R + pixel.G + pixel.B) / 3;
                    suma += (avg - jasnosc) * (avg - jasnosc);
                }
            kontrast = Math.Sqrt(suma / (L * K));
            kontrast_value.Text = Math.Round(kontrast / (255 / 2) * 100, 0) + "%";
            SetBitMap(ref m_ekran);
        }

        //------------------
        //------------------
        public void Kontrast_change()
        {
            System.Drawing.Color pixel;
            int scroll_val = hScrollBar2.Value, r, g, b;
            double suma = 0, jasnosc, kontrast, avg, kontrast_new;
            if (scroll_val <= 0)
                kontrast_new = 1.0 + (scroll_val / 255.0);
            else
                kontrast_new = 255.0 / Math.Pow(2, Math.Log(256 - scroll_val, 2));
            for (int j = 1; j <= L; j++)
                for (int i = 1; i <= K; i++)
                {
                    pixel = m_obraz_w_pamieci.GetPixel(i - 1, j - 1);
                    r = (int)((pixel.R - 255 / 2) * kontrast_new) + 255 / 2;
                    if (r > 255) r = 255;
                    else if (r < 0) r = 0;
                    g = (int)((pixel.G - 255 / 2) * kontrast_new) + 255 / 2;
                    if (g > 255) g = 255;
                    else if (g < 0) g = 0;
                    b = (int)((pixel.B - 255 / 2) * kontrast_new) + 255 / 2;
                    if (b > 255) b = 255;
                    else if (b < 0) b = 0;
                    pixel = System.Drawing.Color.FromArgb(r, g, b);
                    m_ekran.SetPixel(i - 1, j - 1, pixel);
                }
            for (int j = 1; j <= L; j++)
                for (int i = 1; i <= K; i++)
                {
                    pixel = m_ekran.GetPixel(i - 1, j - 1);
                    avg = (pixel.R + pixel.G + pixel.G) / 3;
                    suma += avg;
                }
            jasnosc = suma / (L * K);
            jasnosc_value.Text = Math.Round(jasnosc / 255 * 100, 0) + "%";
            suma = 0;
            for (int j = 1; j <= L; j++)
                for (int i = 1; i <= K; i++)
                {
                    pixel = m_ekran.GetPixel(i - 1, j - 1);
                    avg = (pixel.R + pixel.G + pixel.B) / 3;
                    suma += (avg - jasnosc) * (avg - jasnosc);
                }
            kontrast = Math.Sqrt(suma / (L * K));
            kontrast_value.Text = Math.Round(kontrast / (255 / 2) * 100, 0) + "%";
            SetBitMap(ref m_ekran);
        }

        //------------------
        //------------------
        public void Zmien_obraz()
        {
            System.Drawing.Color pixel;
            for (int j = 1; j <= L; j++)
                for (int i = 1; i <= K; i++)
                {
                    pixel = m_obraz_w_pamieci.GetPixel(i - 1, j - 1);

                    //--------------------------------------//
                    //miejsce na kod dokonuj¹cy zmianê obrazu
                    
                    pixel = System.Drawing.Color.FromArgb(pixel.R, pixel.G, pixel.B);

                    //--------------------------------------//
                    
                    m_ekran.SetPixel(i - 1, j - 1, pixel);
                }
            jasnosc_value.Text = "-";
            kontrast_value.Text = "-";
            SetBitMap(ref m_ekran);
        }
    }
}
