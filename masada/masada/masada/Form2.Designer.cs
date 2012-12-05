namespace masada
{
    partial class Form2
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.typingBoxText = new System.Windows.Forms.TextBox();
            this.typingBoxLabel = new System.Windows.Forms.Label();
            this.logoutButton = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.paragraphText = new System.Windows.Forms.TextBox();
            this.trackBar1 = new System.Windows.Forms.TrackBar();
            this.stdBox = new System.Windows.Forms.TextBox();
            this.profileCheck = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).BeginInit();
            this.SuspendLayout();
            // 
            // typingBoxText
            // 
            this.typingBoxText.Location = new System.Drawing.Point(68, 256);
            this.typingBoxText.Multiline = true;
            this.typingBoxText.Name = "typingBoxText";
            this.typingBoxText.Size = new System.Drawing.Size(668, 243);
            this.typingBoxText.TabIndex = 0;
            this.typingBoxText.KeyDown += new System.Windows.Forms.KeyEventHandler(this.typingBoxText_KeyDown);
            this.typingBoxText.KeyUp += new System.Windows.Forms.KeyEventHandler(this.typingBoxText_KeyUp);
            // 
            // typingBoxLabel
            // 
            this.typingBoxLabel.AutoSize = true;
            this.typingBoxLabel.Location = new System.Drawing.Point(65, 486);
            this.typingBoxLabel.Name = "typingBoxLabel";
            this.typingBoxLabel.Size = new System.Drawing.Size(0, 13);
            this.typingBoxLabel.TabIndex = 1;
            // 
            // logoutButton
            // 
            this.logoutButton.Location = new System.Drawing.Point(704, 12);
            this.logoutButton.Name = "logoutButton";
            this.logoutButton.Size = new System.Drawing.Size(99, 28);
            this.logoutButton.TabIndex = 2;
            this.logoutButton.Text = "Logout";
            this.logoutButton.UseVisualStyleBackColor = true;
            this.logoutButton.Click += new System.EventHandler(this.logoutButton_Click);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(702, 78);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(100, 26);
            this.button1.TabIndex = 4;
            this.button1.Text = "Test Arrays";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.testFilterTimes);
            // 
            // timer1
            // 
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // paragraphText
            // 
            this.paragraphText.Location = new System.Drawing.Point(68, 39);
            this.paragraphText.Multiline = true;
            this.paragraphText.Name = "paragraphText";
            this.paragraphText.Size = new System.Drawing.Size(591, 180);
            this.paragraphText.TabIndex = 5;
            // 
            // trackBar1
            // 
            this.trackBar1.Location = new System.Drawing.Point(348, 537);
            this.trackBar1.Name = "trackBar1";
            this.trackBar1.Size = new System.Drawing.Size(293, 45);
            this.trackBar1.TabIndex = 6;
            this.trackBar1.Scroll += new System.EventHandler(this.trackBar1_Scroll);
            // 
            // stdBox
            // 
            this.stdBox.Location = new System.Drawing.Point(647, 537);
            this.stdBox.Name = "stdBox";
            this.stdBox.Size = new System.Drawing.Size(49, 20);
            this.stdBox.TabIndex = 7;
            // 
            // profileCheck
            // 
            this.profileCheck.Location = new System.Drawing.Point(704, 121);
            this.profileCheck.Name = "profileCheck";
            this.profileCheck.Size = new System.Drawing.Size(98, 40);
            this.profileCheck.TabIndex = 8;
            this.profileCheck.Text = "Check Against Profile";
            this.profileCheck.UseVisualStyleBackColor = true;
            this.profileCheck.Click += new System.EventHandler(this.profileCheck_Click);
            // 
            // Form2
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(815, 605);
            this.Controls.Add(this.profileCheck);
            this.Controls.Add(this.stdBox);
            this.Controls.Add(this.trackBar1);
            this.Controls.Add(this.paragraphText);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.logoutButton);
            this.Controls.Add(this.typingBoxLabel);
            this.Controls.Add(this.typingBoxText);
            this.Name = "Form2";
            this.Text = "Form2";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form2_FormClosing);
            this.Load += new System.EventHandler(this.Form2_Load);
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox typingBoxText;
        private System.Windows.Forms.Label typingBoxLabel;
        private System.Windows.Forms.Button logoutButton;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.TextBox paragraphText;
        private System.Windows.Forms.TrackBar trackBar1;
        private System.Windows.Forms.TextBox stdBox;
        private System.Windows.Forms.Button profileCheck;
    }
}