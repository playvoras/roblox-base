namespace toddlerkick_interface
{
    partial class MainWindow
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
            this.ScriptBox = new System.Windows.Forms.TextBox();
            this.ExecuteButton = new System.Windows.Forms.Button();
            this.ClearButton = new System.Windows.Forms.Button();
            this.AttachButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // ScriptBox
            // 
            this.ScriptBox.Location = new System.Drawing.Point(13, 13);
            this.ScriptBox.Multiline = true;
            this.ScriptBox.Name = "ScriptBox";
            this.ScriptBox.Size = new System.Drawing.Size(459, 215);
            this.ScriptBox.TabIndex = 0;
            this.ScriptBox.Text = "print(\"Hello, world!\")";
            // 
            // ExecuteButton
            // 
            this.ExecuteButton.Location = new System.Drawing.Point(13, 234);
            this.ExecuteButton.Name = "ExecuteButton";
            this.ExecuteButton.Size = new System.Drawing.Size(75, 24);
            this.ExecuteButton.TabIndex = 1;
            this.ExecuteButton.Text = "Execute";
            this.ExecuteButton.UseVisualStyleBackColor = true;
            this.ExecuteButton.Click += new System.EventHandler(this.ExecutionHandler);
            // 
            // ClearButton
            // 
            this.ClearButton.Location = new System.Drawing.Point(94, 234);
            this.ClearButton.Name = "ClearButton";
            this.ClearButton.Size = new System.Drawing.Size(75, 24);
            this.ClearButton.TabIndex = 2;
            this.ClearButton.Text = "Clear";
            this.ClearButton.UseVisualStyleBackColor = true;
            this.ClearButton.Click += new System.EventHandler(this.ExecutionHandler);
            // 
            // AttachButton
            // 
            this.AttachButton.Location = new System.Drawing.Point(397, 234);
            this.AttachButton.Name = "AttachButton";
            this.AttachButton.Size = new System.Drawing.Size(75, 24);
            this.AttachButton.TabIndex = 3;
            this.AttachButton.Text = "Attach";
            this.AttachButton.UseVisualStyleBackColor = true;
            this.AttachButton.Click += new System.EventHandler(this.ExecutionHandler);
            // 
            // MainWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(484, 261);
            this.Controls.Add(this.AttachButton);
            this.Controls.Add(this.ClearButton);
            this.Controls.Add(this.ExecuteButton);
            this.Controls.Add(this.ScriptBox);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.MaximumSize = new System.Drawing.Size(500, 300);
            this.MinimumSize = new System.Drawing.Size(500, 300);
            this.Name = "MainWindow";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "ToddlerKick";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox ScriptBox;
        private System.Windows.Forms.Button ExecuteButton;
        private System.Windows.Forms.Button ClearButton;
        private System.Windows.Forms.Button AttachButton;
    }
}

